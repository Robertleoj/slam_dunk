#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <slamd/slamd.hpp>

namespace py = pybind11;

namespace pybind11::detail {

template <>
struct type_caster<glm::vec2> {
    PYBIND11_TYPE_CASTER(glm::vec2, _("numpy.ndarray[float32[2]]"));

    bool load(
        handle src,
        bool
    ) {
        auto buf =
            py::array_t<float, py::array::c_style | py::array::forcecast>::
                ensure(src);
        if (!buf || buf.ndim() != 1 || buf.shape(0) != 2) {
            return false;
        }
        value = glm::vec2(buf.at(0), buf.at(1));
        return true;
    }

    static handle cast(
        const glm::vec2& v,
        return_value_policy,
        handle
    ) {
        return py::array_t<float>({2}, {sizeof(float)}, &v[0]).release();
    }
};

template <>
struct type_caster<glm::vec3> {
    PYBIND11_TYPE_CASTER(glm::vec3, _("numpy.ndarray[float32[3]]"));

    bool load(
        handle src,
        bool
    ) {
        auto buf =
            py::array_t<float, py::array::c_style | py::array::forcecast>::
                ensure(src);
        if (!buf || buf.ndim() != 1 || buf.shape(0) != 3) {
            return false;
        }
        value = glm::vec3(buf.at(0), buf.at(1), buf.at(2));
        return true;
    }

    static handle cast(
        const glm::vec3& v,
        return_value_policy,
        handle
    ) {
        return py::array_t<float>({3}, {sizeof(float)}, &v[0]).release();
    }
};

template <>
struct type_caster<glm::mat4> {
    PYBIND11_TYPE_CASTER(glm::mat4, _("numpy.ndarray[float32[4][4]]"));

    bool load(
        handle src,
        bool
    ) {
        auto buf =
            py::array_t<float, py::array::c_style | py::array::forcecast>::
                ensure(src);
        if (!buf || buf.ndim() != 2 || buf.shape(0) != 4 || buf.shape(1) != 4) {
            return false;
        }
        glm::mat4 m(1.0f);
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m[j][i] = buf.at(i, j);  // GLM column-major
            }
        }
        value = m;
        return true;
    }

    static handle cast(
        const glm::mat4& m,
        return_value_policy,
        handle
    ) {
        return py::array_t<float>(
                   {4, 4},
                   {sizeof(float) * 4, sizeof(float)},
                   &m[0][0]
        )
            .release();
    }
};

template <>
struct type_caster<glm::mat3> {
    PYBIND11_TYPE_CASTER(glm::mat3, _("numpy.ndarray[float32[3][3]]"));

    bool load(
        handle src,
        bool
    ) {
        auto buf =
            py::array_t<float, py::array::c_style | py::array::forcecast>::
                ensure(src);
        if (!buf || buf.ndim() != 2 || buf.shape(0) != 3 || buf.shape(1) != 3) {
            return false;
        }
        glm::mat4 m(1.0f);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                m[j][i] = buf.at(i, j);  // GLM column-major
            }
        }
        value = m;
        return true;
    }

    static handle cast(
        const glm::mat4& m,
        return_value_policy,
        handle
    ) {
        return py::array_t<float>(
                   {3, 3},
                   {sizeof(float) * 3, sizeof(float)},
                   &m[0][0]
        )
            .release();
    }
};

// vector<glm::vec3>
template <>
struct type_caster<std::vector<glm::vec3>> {
    PYBIND11_TYPE_CASTER(
        std::vector<glm::vec3>,
        _("numpy.ndarray[float32[n][3]]")
    );

    bool load(
        handle src,
        bool
    ) {
        auto buf =
            py::array_t<float, py::array::c_style | py::array::forcecast>::
                ensure(src);
        if (!buf || buf.ndim() != 2 || buf.shape(1) != 3) {
            return false;
        }
        ssize_t n = buf.shape(0);
        value.resize(n);
        for (ssize_t i = 0; i < n; ++i) {
            value[i] = glm::vec3(buf.at(i, 0), buf.at(i, 1), buf.at(i, 2));
        }
        return true;
    }

    static handle cast(
        const std::vector<glm::vec3>& vecs,
        return_value_policy,
        handle
    ) {
        std::vector<ssize_t> shape = {static_cast<ssize_t>(vecs.size()), 3};
        std::vector<ssize_t> strides = {
            static_cast<ssize_t>(sizeof(float) * 3),
            static_cast<ssize_t>(sizeof(float))
        };

        py::array_t<float> arr(
            py::buffer_info(
                nullptr,                                 // no data yet
                sizeof(float),                           // size of one float
                py::format_descriptor<float>::format(),  // format string
                2,                                       // ndim
                shape,                                   // shape
                strides                                  // strides
            )
        );

        auto r = arr.mutable_unchecked<2>();
        for (size_t i = 0; i < vecs.size(); ++i) {
            for (int j = 0; j < 3; ++j) {
                r(i, j) = vecs[i][j];
            }
        }

        return arr.release();
    }
};

// vector<glm::vec2>
template <>
struct type_caster<std::vector<glm::vec2>> {
    PYBIND11_TYPE_CASTER(
        std::vector<glm::vec2>,
        _("numpy.ndarray[float32[n][2]]")
    );

    bool load(
        handle src,
        bool
    ) {
        auto buf =
            py::array_t<float, py::array::c_style | py::array::forcecast>::
                ensure(src);
        if (!buf || buf.ndim() != 2 || buf.shape(1) != 2) {
            return false;
        }
        ssize_t n = buf.shape(0);
        value.resize(n);
        for (ssize_t i = 0; i < n; ++i) {
            value[i] = glm::vec2(buf.at(i, 0), buf.at(i, 1));
        }
        return true;
    }

    static handle cast(
        const std::vector<glm::vec2>& vecs,
        return_value_policy,
        handle
    ) {
        std::vector<ssize_t> shape = {static_cast<ssize_t>(vecs.size()), 2};
        std::vector<ssize_t> strides = {
            static_cast<ssize_t>(sizeof(float) * 2),
            static_cast<ssize_t>(sizeof(float))
        };

        py::array_t<float> arr(
            py::buffer_info(
                nullptr,                                 // no data yet
                sizeof(float),                           // item size
                py::format_descriptor<float>::format(),  // format string
                2,                                       // ndim
                shape,                                   // shape
                strides                                  // strides
            )
        );

        auto r = arr.mutable_unchecked<2>();
        for (size_t i = 0; i < vecs.size(); ++i) {
            for (int j = 0; j < 2; ++j) {
                r(i, j) = vecs[i][j];
            }
        }

        return arr.release();
    }
};

template <>
struct type_caster<slamd::data::Image> {
    PYBIND11_TYPE_CASTER(
        slamd::data::Image,
        _("numpy.ndarray[uint8[H][W][C]]")
    );

    bool load(
        handle src,
        bool
    ) {
        // Ensure it's a 3D numpy array of uint8
        auto buf =
            py::array_t<uint8_t, py::array::c_style | py::array::forcecast>::
                ensure(src);
        if (!buf || buf.ndim() != 3) {
            return false;
        }

        size_t height = buf.shape(0);
        size_t width = buf.shape(1);
        size_t channels = buf.shape(2);

        std::vector<uint8_t> data(buf.size());
        std::memcpy(data.data(), buf.data(), buf.size());

        value = slamd::data::Image{std::move(data), width, height, channels};
        return true;
    }

    static handle cast(
        const slamd::data::Image&,
        return_value_policy,
        handle
    ) {
        throw std::runtime_error("Casting data::Image → numpy not supported");
    }
};

}  // namespace pybind11::detail

void define_private_geom(
    py::module_& m
) {
    py::class_<slamd::_geom::Geometry, std::shared_ptr<slamd::_geom::Geometry>>(
        m,
        "Geometry"
    );

    py::class_<
        slamd::geom::Box,
        slamd::_geom::Geometry,
        std::shared_ptr<slamd::geom::Box>>(m, "Box");

    py::class_<
        slamd::geom::Arrows,
        slamd::_geom::Geometry,
        std::shared_ptr<slamd::geom::Arrows>>(m, "Arrows");

    py::class_<
        slamd::geom::CameraFrustum,
        slamd::_geom::Geometry,
        std::shared_ptr<slamd::geom::CameraFrustum>>(m, "CameraFrustum");

    py::class_<
        slamd::geom::MonoMesh,
        slamd::_geom::Geometry,
        std::shared_ptr<slamd::geom::MonoMesh>>(m, "MonoMesh");

    py::class_<
        slamd::_geom::PointCloud,
        slamd::_geom::Geometry,
        std::shared_ptr<slamd::_geom::PointCloud>>(m, "PointCloud")
        .def(
            "update_positions",
            &slamd::_geom::PointCloud::update_positions,
            py::arg("positions")
        )
        .def(
            "update_colors",
            &slamd::_geom::PointCloud::update_colors,
            py::arg("colors")
        )
        .def(
            "update_radii",
            &slamd::_geom::PointCloud::update_radii,
            py::arg("radii")
        );

    py::class_<
        slamd::geom2d::Points2D,
        slamd::_geom::Geometry,
        std::shared_ptr<slamd::geom2d::Points2D>>(m, "Points2D");

    py::class_<
        slamd::geom::PolyLine,
        slamd::_geom::Geometry,
        std::shared_ptr<slamd::geom::PolyLine>>(m, "PolyLine");

    py::class_<
        slamd::geom::SimpleMesh,
        slamd::_geom::Geometry,
        std::shared_ptr<slamd::geom::SimpleMesh>>(m, "SimpleMesh");

    py::class_<
        slamd::geom::Sphere,
        slamd::_geom::Geometry,
        std::shared_ptr<slamd::geom::Sphere>>(m, "Sphere");

    py::class_<
        slamd::geom::Triad,
        slamd::_geom::Geometry,
        std::shared_ptr<slamd::geom::Triad>>(m, "Triad");

    py::class_<
        slamd::geom::Image,
        slamd::_geom::Geometry,
        std::shared_ptr<slamd::geom::Image>>(m, "Image");
}

void define_geom(
    py::module_& m
) {
    m.def("box", &slamd::geom::box, "Create a Box geometry");
    m.def(
        "arrows",
        &slamd::geom::arrows,
        py::arg("starts"),
        py::arg("ends"),
        py::arg("colors"),
        py::arg("thickness"),
        "Create an Arrows geometry"
    );

    m.def(
        "camera_frustum",
        [](glm::mat3 intrinsics_matrix,
           size_t image_width,
           size_t image_height,
           std::optional<slamd::data::Image> image,
           float scale = 1.0) {
            if (!image.has_value()) {
                return slamd::geom::camera_frustum(
                    intrinsics_matrix,
                    image_width,
                    image_height,
                    scale
                );
            }
            return slamd::geom::camera_frustum(
                intrinsics_matrix,
                image_width,
                image_height,
                std::move(image.value()),
                scale
            );
        },
        py::arg("intrinsics_matrix"),
        py::arg("image_width"),
        py::arg("image_height"),
        py::arg("image") = std::nullopt,
        py::arg("scale") = 1.0f,
        "Create a CameraFrustum geometry"
    );

    m.def(
        "mono_mesh",
        &slamd::geom::mono_mesh,
        py::arg("vertices"),
        py::arg("triangle_indices"),
        py::arg("color"),
        "Create a MonoMesh geometry"
    );

    // Overload: uniform color + uniform radius
    m.def(
        "point_cloud",
        [](const std::vector<glm::vec3>& positions,
           const glm::vec3& color,
           float radius) {
            return slamd::geom::point_cloud(positions, color, radius);
        },
        py::arg("positions"),
        py::arg("color"),
        py::arg("radius"),
        "Create a PointCloud with uniform color and radius"
    );

    // Overload: per-point color + per-point radius
    m.def(
        "point_cloud",
        [](const std::vector<glm::vec3>& positions,
           const std::vector<glm::vec3>& colors,
           const std::vector<float>& radii) {
            return slamd::geom::point_cloud(positions, colors, radii);
        },
        py::arg("positions"),
        py::arg("colors"),
        py::arg("radii"),
        "Create a PointCloud with per-point color and radius"
    );

    m.def(
        "poly_line",
        &slamd::geom::poly_line,
        py::arg("points"),
        py::arg("thickness"),
        py::arg("color"),
        "Create a PolyLine geometry"
    );

    // Explicitly cast the version we want
    std::shared_ptr<slamd::geom::SimpleMesh> (*simple_mesh_from_raw)(
        const std::vector<glm::vec3>&,
        const std::vector<glm::vec3>&,
        const std::vector<uint32_t>&
    ) = &slamd::geom::simple_mesh;

    m.def(
        "simple_mesh",
        simple_mesh_from_raw,
        py::arg("vertices"),
        py::arg("vertex_colors"),
        py::arg("triangle_indices"),
        "Create a SimpleMesh geometry from raw data"
    );

    m.def(
        "sphere",
        &slamd::geom::sphere,
        py::arg("radius") = 1.0f,
        py::arg("color") = glm::vec3(0.8f, 0.2f, 0.0f),
        "Create a Sphere geometry"
    );

    m.def(
        "triad",
        &slamd::geom::triad,
        py::arg("scale") = 1.0f,
        py::arg("thickness") = 0.1f,
        "Create a Triad geometry"
    );
}

void define_geom2d(
    py::module_& m
) {
    // Overload: uniform color + radius
    m.def(
        "points_2d",
        [](const std::vector<glm::vec2>& positions,
           const glm::vec3& color,
           float radius) {
            return slamd::geom2d::points_2d(positions, color, radius);
        },
        py::arg("positions"),
        py::arg("color"),
        py::arg("radius"),
        "Create 2D points with uniform color and radius"
    );

    // Overload: per-point color + radius
    m.def(
        "points_2d",
        [](const std::vector<glm::vec2>& positions,
           const std::vector<glm::vec3>& colors,
           const std::vector<float>& radii) {
            return slamd::geom2d::points_2d(positions, colors, radii);
        },
        py::arg("positions"),
        py::arg("colors"),
        py::arg("radii"),
        "Create 2D points with per-point color and radius"
    );

    m.def(
        "image",
        [](slamd::data::Image img) {
            return slamd::geom2d::image(std::move(img));
        },
        py::arg("image"),
        "Create an Image geometry from a NumPy uint8 array (H, W, C)"
    );
}

PYBIND11_MODULE(
    bindings,
    m
) {
    m.doc() = "SlamDunk visualization library";

    py::class_<slamd::Scene, std::shared_ptr<slamd::Scene>>(m, "Scene")
        .def(
            "set_transform",
            &slamd::Scene::set_transform,
            py::arg("path"),
            py::arg("transform")
        )
        .def(
            "set_object",
            &slamd::_tree::Tree::set_object,
            py::arg("path"),
            py::arg("object")
        );

    m.def("scene", &slamd::scene);

    // Canvas bindings
    py::class_<slamd::Canvas, std::shared_ptr<slamd::Canvas>>(m, "Canvas")
        .def(
            "set_transform",
            &slamd::Canvas::set_transform,
            py::arg("path"),
            py::arg("transform")
        )
        .def(
            "set_object",
            &slamd::Canvas::set_object,
            py::arg("path"),
            py::arg("object")
        );

    m.def("canvas", &slamd::canvas);

    py::class_<slamd::Window>(m, "Window")
        .def(
            py::init<std::string, size_t, size_t>(),
            py::arg("name"),
            py::arg("height"),
            py::arg("width")
        )
        .def("wait_for_close", &slamd::Window::wait_for_close)
        .def(
            "add_scene",
            &slamd::Window::add_scene,
            py::arg("name"),
            py::arg("scene")
        )
        .def(
            "add_canvas",
            &slamd::Window::add_canvas,
            py::arg("name"),
            py::arg("canvas")
        );

    auto _geom = m.def_submodule("_geom");
    define_private_geom(_geom);

    auto geom = m.def_submodule("geom");
    define_geom(geom);

    auto geom2d = m.def_submodule("geom2d");
    define_geom2d(geom2d);
}
