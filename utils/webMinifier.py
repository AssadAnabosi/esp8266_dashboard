import shutil
import subprocess
import sys
from pathlib import Path

# Ensure the required library is installed
try:
    from css_html_js_minify import process_single_css_file, process_single_html_file, process_single_js_file
except ImportError:
    print("Required library 'css_html_js_minify' is not installed.")
    approval = input("Do you want to install it now? (y/n): ").strip().lower()
    if approval in ["yes", "y"]:
        subprocess.check_call([sys.executable, "-m", "pip", "install", "css-html-js-minify"])
        from css_html_js_minify import process_single_css_file, process_single_html_file, process_single_js_file
    else:
        print("Library installation declined. Exiting.")
        exit(1)

# Prompt the user to select the target platform
print("Select the target platform:")
print("1. ESP8266")
print("2. ESP32")
choice = input("Enter 1 or 2: ").strip()

if choice == "1":
    output_dir = Path(__file__).parent / "../esp8266/data/web"
elif choice == "2":
    output_dir = Path(__file__).parent / "../esp32/data/web"
else:
    print("Invalid choice. Exiting.")
    exit(1)

# Check if the output directory exists and is not empty
if output_dir.exists() and any(output_dir.iterdir()):
    print(f"The output directory '{output_dir}' is not empty.")
    approval = input("Do you approve clearing it? (y/n): ").strip().lower()
    if approval not in ["yes", "y"]:
        print("Operation canceled by the user.")
        exit(1)
    shutil.rmtree(output_dir)  # Clear the destination directory

# Ensure the output directories exist
output_dir.mkdir(parents=True, exist_ok=True)
output_js_dir = output_dir / "js"
output_js_dir.mkdir(parents=True, exist_ok=True)

def minify_file(file_path):
    """Minify a file in place based on its type."""
    if file_path.suffix == ".css":
        process_single_css_file(file_path, overwrite=True)
    elif file_path.suffix == ".html":
        process_single_html_file(file_path, overwrite=True)
    elif file_path.suffix == ".js":
        process_single_js_file(file_path, overwrite=True)
    else:
        print(f"Skipping unsupported file: {file_path}")
        return

# Define source directories
html_css_src = Path(__file__).parent / "../web-interface"
js_src = html_css_src / "js"

# Copy only .html, .css, and .js files from the source to the destination directory
for file in html_css_src.rglob("*"):
    if file.suffix in [".html", ".css", ".js"]:
        relative_path = file.relative_to(html_css_src)
        destination = output_dir / relative_path
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(file, destination)

# Minify HTML and CSS files in the destination directory
for file in output_dir.glob("*"):
    if file.suffix in [".css", ".html"]:
        minify_file(file)

# Causes issues with the minifier, so we are not using it for now

# # Minify JavaScript files in the "js" subdirectory of the destination directory
# if output_js_dir.exists():
#     for js_file in output_js_dir.glob("*.js"):
#         minify_file(js_file)