from pathlib import Path
from jinja2 import Environment, FileSystemLoader
from dataclasses import dataclass, asdict


def repo_root():
    curr_path = Path(__file__)
    while not (curr_path / ".git").exists():
        curr_path = curr_path.parent
    return curr_path.resolve()


@dataclass
class Shader:
    namespace: str
    vertex_shader: str
    fragment_shader: str


def collect_shaders(shader_root: Path) -> list[Shader]:
    shaders: list[Shader] = []

    for subdir in sorted(Path(shader_root).iterdir()):
        if not subdir.is_dir():
            continue

        vert_path = subdir / "vertex_shader.vert"
        vert_source = vert_path.read_text()

        frag_path = subdir / "fragment_shader.frag"
        frag_source = frag_path.read_text()

        shaders.append(Shader(subdir.name, vert_source, frag_source))

    return shaders


def render_header(shaders: list[Shader], template_path: Path, output_path: Path):
    env = Environment(loader=FileSystemLoader(template_path.parent))

    template = env.get_template(template_path.name)

    dict_list = [asdict(s) for s in shaders]

    rendered = template.render(shaders=dict_list)

    with open(output_path, "w") as f:
        f.write(rendered)


def embed_shaders():
    shader_dir = repo_root() / Path("slamd/shaders")
    output_file = repo_root() / Path("slamd/include/slamd/gen/shader_sources.hpp")
    template_file = repo_root() / Path("templates/shader_template.hpp.j2")

    shader_data = collect_shaders(shader_dir)
    render_header(shader_data, template_file, output_file)
    print(f"✅ Header written to {output_file}")


if __name__ == "__main__":
    embed_shaders()
