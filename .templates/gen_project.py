from argparse import ArgumentParser, ArgumentTypeError
from enum import Enum, unique
import shutil
from pathlib import Path
import os
import atexit

PROJECT_TYPE = {
    0: "cxx_exe",
    1: "cxx_lib",
    2: "cuda_exe",
    3: "cuda_lib",
}

TEMPLATE_DIR = Path(".templates")
GITHUB_WORKFLOWS_DIR = Path(".github/workflows")
LICENSE_PATH = Path("LICENSE")
README_PATH = Path("README.md")

TEMPLATE_GEN_PATHS = [
    "cmake",
    "include",
    "src",
    "lib",
    "test",
    "scripts",
    "CMakeLists.txt",
    ".clangd",
    ".clang-format",
    ".github/workflows/ci-auto-format-and-commit.yml"
    ".github/workflows/ci-build-and-test.yml",
    "vcpkg.json",
]

TARGET_EXTENSIONS = (
    ".c",
    ".h",
    ".cpp",
    ".hpp",
    ".cu",
    ".cuh",
    "CMakeLists.txt",
    ".cmake",
)

MATCH_PATTERN = "_template_project_name_"


class ProjectGenerator:
    def __init__(self, project_name: str, project_type: str):
        self.project_name = project_name
        self.project_type = PROJECT_TYPE[project_type]

    def run(self):
        shutil.copytree(TEMPLATE_DIR / "common", ".", dirs_exist_ok=True)
        shutil.copytree(TEMPLATE_DIR / self.project_type, ".", dirs_exist_ok=True)
        Path("./include", MATCH_PATTERN).rename(Path("./include", self.project_name))
        for directory in list(map(Path, ["include", "lib", "test", "src", "cmake"])):
            self.replace_in_directory(directory, MATCH_PATTERN, self.project_name)
        self.replace_api()
        self.replace_in_file("CMakeLists.txt", MATCH_PATTERN, self.project_name)
        if self.project_type.startswith("cuda"):
            if not (cuda_home := os.environ.get("CUDA_HOME", None)):
                cuda_home = os.environ.get("CUDA_DIR", None)
            self.replace_in_file(".clangd", "<path-to-cuda>", cuda_home)
        else:
            self.remove_line_in_file(".clangd", "cuda")

    @staticmethod
    def replace_in_file(file_path: str, old: str, new: str):
        file_path = Path(file_path)
        with open(file_path, "r") as file:
            content = file.read()
        content = content.replace(old, new)
        with open(file_path, "w") as file:
            file.write(content)

    @staticmethod
    def remove_line_in_file(file_path: str, pattern: str):
        file_path = Path(file_path)

        # Read the content of the file
        with open(file_path, "r") as file:
            lines = file.readlines()

        # Write back all lines except those containing 'pattern'
        with open(file_path, "w") as file:
            for line in lines:
                if pattern not in line:
                    file.write(line)

    @staticmethod
    def replace_in_directory(directory, old, new):
        for root, _, files in Path(directory).walk():
            for file in files:
                if file.endswith(TARGET_EXTENSIONS):
                    file_path = Path(root, file)
                    ProjectGenerator.replace_in_file(file_path, old, new)

    def replace_api(self):
        self.replace_in_directory(
            Path("include"),
            f"{self.project_name}_API",
            f"{self.project_name.upper()}_API",
        )
        self.replace_in_directory(
            Path("include"),
            f"{self.project_name}_EXPORT",
            f"{self.project_name.upper()}_EXPORT",
        )
        self.replace_in_directory(
            Path("include"),
            f"{self.project_name}_IMPORT",
            f"{self.project_name.upper()}_IMPORT",
        )


def main(args):
    # Remove ".templates" directory on exit
    if args.remove_templates:
        to_remove = [
            TEMPLATE_DIR,
            GITHUB_WORKFLOWS_DIR,
            LICENSE_PATH,
            README_PATH,
        ]
        atexit.register(
            lambda: [shutil.rmtree(path, ignore_errors=True) for path in to_remove]
        )
        return

    # Reset project directory by removing all template-generated file
    for path in TEMPLATE_GEN_PATHS:
        path = Path(path)
        if path.is_file():
            path.unlink(missing_ok=True)
        elif path.is_dir():
            shutil.rmtree(path, ignore_errors=True)

    # Reset only
    if args.reset:
        shutil.copytree(TEMPLATE_DIR / "reset", ".", dirs_exist_ok=True)
        return

    generator = ProjectGenerator(args.project_name, args.project_type)
    generator.run()


if __name__ == "__main__":
    parser = ArgumentParser(description="Project type selector")

    parser.add_argument(
        "--remove-templates",
        action="store_true",
        help="Remove '.templates' and '.github/workflows'.",
    )

    parser.add_argument(
        "--reset",
        action="store_true",
        help="Reset template, remove all generated files",
    )

    parser.add_argument(
        "-t",
        "--project-type",
        type=int,
        choices=[t for t in range(len(PROJECT_TYPE))],
        help=f"Type of project to create: {PROJECT_TYPE}",
    )

    def validate_project_name(name: str):
        if not name.isidentifier():
            raise ArgumentTypeError(f"Project name '{name}' must be a valid identifier")
        return name

    parser.add_argument(
        "-n",
        "--project-name",
        type=validate_project_name,
        help="Name of the project. Affects namespace and the include directory name.",
    )

    args = parser.parse_args()

    if not (args.reset or args.remove_templates) and (
        args.project_type is None or args.project_name is None
    ):
        parser.error(
            "--project-type and --project-name are required unless "
            "--remove-templates or --reset is specified"
        )

    main(args)
