import shutil
from pathlib import Path
from css_html_js_minify import process_single_css_file, process_single_html_file, process_single_js_file

# Define source and destination directories
html_css_src = Path(__file__).parent / "../web-interface"
js_src = html_css_src / "js"
output_dir = Path(__file__).parent / "../esp-dashboard/data/web"
output_js_dir = output_dir / "js"

# Ensure the output directories exist
output_dir.mkdir(parents=True, exist_ok=True)
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

# Copy only .html, .css, and .js files from the source to the destination directory
if output_dir.exists():
    shutil.rmtree(output_dir)  # Clear the destination directory if it exists
output_dir.mkdir(parents=True, exist_ok=True)

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