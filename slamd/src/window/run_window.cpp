#include <glbinding/gl/gl.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
#include <slamd_window/frame_timer.hpp>
#include <slamd_window/glfw.hpp>
#include <slamd_window/run_window.hpp>

namespace slamd {

void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height
) {
    (void)window;
    gl::glViewport(0, 0, width, height);
}

inline void tree_menu(
    Node* root,
    ImGuiTreeNodeFlags node_flags = 0
) {
    static char filter_buf[128] = "";  // text input buffer (persists)

    // Compute a field width that fully shows the text (plus padding)
    const float min_field_w = 250.0f;
    const float text_w = ImGui::CalcTextSize(filter_buf).x;
    const float pad_x = ImGui::GetStyle().FramePadding.x;

    // A little extra so the caret isn’t jammed at the edge
    const float desired_field_w = text_w + 2.0f * pad_x + 12.0f;
    const float field_w =
        (desired_field_w < min_field_w) ? min_field_w : desired_field_w;

    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted("Filter:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(field_w);  // take all remaining width on the line
    ImGui::InputText("##filter", filter_buf, IM_ARRAYSIZE(filter_buf));
    ImGui::Separator();

    // --- Text input box above tree ---
    // ImGui::InputText("Filter:##filter", filter_buf,
    // IM_ARRAYSIZE(filter_buf)); ImGui::Separator();

    std::function<void(Node*, std::string, int)> draw_node =
        [&](Node* n, std::string label, int depth) {
            ImGui::PushID(n);  // stable-ish ID; swap for n->id if you got one

            const bool has_children = !n->children.empty();
            const ImGuiTreeNodeFlags base_flags =
                ImGuiTreeNodeFlags_SpanAvailWidth |
                ImGuiTreeNodeFlags_FramePadding |
                (has_children ? 0
                              : ImGuiTreeNodeFlags_Leaf |
                                    ImGuiTreeNodeFlags_NoTreePushOnOpen);

            // Checkbox aligned to the right of the same line
            bool visible = true;
            ImGui::Checkbox("##visible", &visible);
            ImGui::SameLine(0.0f, 4.0f);

            bool open = false;
            if (has_children) {
                open =
                    ImGui::TreeNodeEx(label.c_str(), base_flags | node_flags);
            } else {
                ImGui::TreeNodeEx(label.c_str(), base_flags | node_flags);
            }

            // Optional context menu
            if (ImGui::BeginPopupContextItem("tree_ctx")) {
                ImGui::TextUnformatted(label.c_str());
                ImGui::Separator();
                if (ImGui::MenuItem("Select")) {
                }
                ImGui::EndPopup();
            }

            if (open) {
                for (const auto& c : n->children) {
                    draw_node(c.second.get(), c.first, depth + 1);
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        };

    draw_node(root, "/", 0);
}

inline void draw_tree_overlay(
    Node* root,
    const char* overlay_id = "##scene_tree_overlay",
    const char* header = "Tree",
    float margin = 8.0f,
    float min_width = 100.0f
) {
    // Anchor to current window's content region (screen coords)
    ImVec2 win_pos = ImGui::GetWindowPos();
    ImVec2 cr_min = ImGui::GetWindowContentRegionMin();
    ImVec2 cr_max = ImGui::GetWindowContentRegionMax();
    ImVec2 tl(win_pos.x + cr_min.x, win_pos.y + cr_min.y);
    ImVec2 br(win_pos.x + cr_max.x, win_pos.y + cr_max.y);

    ImVec2 pos(tl.x + margin, tl.y + margin);
    ImVec2 max_size(br.x - tl.x - 2 * margin, br.y - tl.y - 2 * margin);

    // Style: dark translucent bg, rounded corners, slim padding
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6, 6));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.55f));

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0, 0));
    ImGui::SetNextWindowViewport(ImGui::GetWindowViewport()->ID);
    ImGui::SetNextWindowSizeConstraints(ImVec2(min_width, 0), max_size);

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;  // pinned

    ImGui::Begin(overlay_id, nullptr, flags);

    if (header && *header) {
        ImGui::TextUnformatted(header);
        ImGui::Separator();
    }

    // Reuse your tree renderer (defaults collapsed)
    tree_menu(
        root,
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth
    );

    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);
}

void run_window(
    StateManager& state_manager
) {
    auto window = glutils::make_window("Slam Dunk", 1000, 1000);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    FrameTimer frame_timer;
    constexpr double target_frame_time = 1.0 / 120.0;  // ~0.00833 seconds
    bool loaded_layout = false;
    bool checked_layout = false;

    while (!glfwWindowShouldClose(window)) {
        state_manager.apply_updates();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (!state_manager.loaded) {
            // render a window with a message
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_Always);
            ImGui::Begin("Waiting for Connection", nullptr);
            ImGui::TextWrapped(
                "Hold tight, bro! We're just waiting for that connection to "
                "come through..."
            );
            ImGui::End();

        } else {
            ImGuiID main_dockspace_id;

            if (!loaded_layout && !checked_layout) {
                if (state_manager.layout_path.has_value()) {
                    auto layout_path = state_manager.layout_path.value();
                    if (fs::exists(layout_path)) {
                        ImGui::LoadIniSettingsFromDisk(
                            layout_path.string().c_str()
                        );
                        loaded_layout = true;
                    }
                    checked_layout = true;
                }
            }

            main_dockspace_id = ImGui::DockSpaceOverViewport();

            for (auto& [scene_name, scene] : state_manager.views) {
                if (!loaded_layout) {
                    ImGui::SetNextWindowDockID(
                        main_dockspace_id,
                        ImGuiCond_Once
                    );
                }
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

                ImGui::Begin(
                    scene_name.c_str(),
                    nullptr,
                    ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoScrollWithMouse
                );
                scene->render_to_imgui();

                draw_tree_overlay(scene->tree->root.get());

                ImGui::End();
                ImGui::PopStyleVar();
            }
        }

        ImGui::Render();

        gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        gl::glClear(gl::GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();

        frame_timer.log_frame();
        float frame_time = frame_timer.timedelta();

        if (frame_time < target_frame_time) {
            float sleep_duration = target_frame_time - frame_time;
            std::this_thread::sleep_for(
                std::chrono::duration<float>(sleep_duration)
            );
        }
    }

    SPDLOG_INFO("Window closed!");

    if (state_manager.layout_path.has_value()) {
        auto layout_path = state_manager.layout_path.value();
        SPDLOG_INFO("Saving layout to {}", layout_path.string());
        ImGui::SaveIniSettingsToDisk(layout_path.string().c_str());
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}
}  // namespace slamd