#pragma once

namespace slamd {

class View {
   public:
    virtual void render_to_imgui() = 0;  // pure virtual = abstract base class
    virtual ~View() = default;  // virtual dtor for safe polymorphic deletion
};

}  // namespace slamd