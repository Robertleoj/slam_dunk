![](./images/logo.png)

---

SlamDunk is a lightweight 3D visualization library targeting SLAM applications.

You provide an object tree where you can insert objects and transforms, and SlamDunk then renders them in a beautiful scene with intuitive controls.

We use ImGui to allow panel layouts for multiple scenes and visualizations.

For simple usage, you can allow SlamDunk to handle the render loop. In this scenario, you can create basic panel layouts for different visualizers.

For more advanced usage, you can create your own render loop, and instruct SlamDunk to render into ImGui windows.

For easy integration into your SLAM system, we support using weak pointers to scene objects in our scene tree, allowing you to own the scene object. You can modify it deep in your program, and it updates correctly in the viewer. Then, if your owner is deleted, the object is seemlessly deleted from the scene.

---

SlamDunk is in very early development - demos, documentation, and usage examples are coming soon.
