from flask import Flask, render_template, request, send_file, jsonify
import subprocess
import os
import uuid
import hashlib

app = Flask(
    __name__,
    template_folder="web/templates",
    static_folder="web/static"
)

UPLOAD_DIR = "uploads"
ENGINE = os.path.join(os.getcwd(), "nexus_engine")

os.makedirs(UPLOAD_DIR, exist_ok=True)


def sha256_file(path):
    h = hashlib.sha256()

    with open(path, "rb") as f:
        while True:
            chunk = f.read(1024 * 1024)
            if not chunk:
                break
            h.update(chunk)

    return h.hexdigest()


@app.route("/")
def home():
    return render_template("index.html")


@app.route("/analyze", methods=["POST"])
def analyze():
    if "file" not in request.files:
        return jsonify({"error": "No file uploaded"}), 400

    uploaded_file = request.files["file"]

    if uploaded_file.filename == "":
        return jsonify({"error": "No file selected"}), 400

    unique_id = str(uuid.uuid4())
    input_path = os.path.join(
        UPLOAD_DIR,
        unique_id + "_analyze"
    )

    uploaded_file.save(input_path)

    result = subprocess.run(
        [
            ENGINE,
            "analyze",
            input_path
        ],
        capture_output=True,
        text=True
    )

    try:
        os.remove(input_path)
    except OSError:
        pass

    if result.returncode != 0:
        return jsonify({
            "error": result.stderr or "Analysis failed"
        }), 500

    analysis = {}

    for line in result.stdout.strip().splitlines():
        if "=" in line:
            key, value = line.split("=", 1)
            analysis[key] = value

    return jsonify(analysis)


@app.route("/compress", methods=["POST"])
def compress():
    if "file" not in request.files:
        return jsonify({"error": "No file uploaded"}), 400

    uploaded_file = request.files["file"]

    if uploaded_file.filename == "":
        return jsonify({"error": "No file selected"}), 400

    original_name = os.path.basename(uploaded_file.filename)
    unique_id = str(uuid.uuid4())

    input_path = os.path.join(
        UPLOAD_DIR,
        unique_id + "_input"
    )

    output_path = os.path.join(
        UPLOAD_DIR,
        unique_id + ".huff"
    )

    uploaded_file.save(input_path)

    original_hash = sha256_file(input_path)

    result = subprocess.run(
        [
            ENGINE,
            "compress",
            input_path,
            output_path
        ],
        capture_output=True,
        text=True
    )

    if result.returncode != 0:
        return jsonify({
            "error": result.stderr or "Compression failed"
        }), 500

    response = send_file(
        output_path,
        as_attachment=True,
        download_name=original_name + ".huff"
    )

    response.headers["X-Original-Name"] = original_name
    response.headers["X-Original-SHA256"] = original_hash

    return response


@app.route("/decompress", methods=["POST"])
def decompress():
    if "file" not in request.files:
        return jsonify({"error": "No file uploaded"}), 400

    uploaded_file = request.files["file"]

    if uploaded_file.filename == "":
        return jsonify({"error": "No file selected"}), 400

    unique_id = str(uuid.uuid4())

    input_path = os.path.join(
        UPLOAD_DIR,
        unique_id + ".huff"
    )

    output_dir = os.path.join(
        UPLOAD_DIR,
        unique_id + "_restored"
    )

    os.makedirs(output_dir, exist_ok=True)

    uploaded_file.save(input_path)

    result = subprocess.run(
        [
            ENGINE,
            "decompress",
            input_path,
            output_dir
        ],
        capture_output=True,
        text=True
    )

    if result.returncode != 0:
        return jsonify({
            "error": result.stderr or "Decompression failed"
        }), 500

    restored_path = None

    for line in result.stdout.splitlines():
        if line.startswith("RESTORED_PATH="):
            restored_path = line.split("=", 1)[1].strip()
            break

    if not restored_path or not os.path.exists(restored_path):
        return jsonify({
            "error": "Restored file was not found"
        }), 500

    restored_name = os.path.basename(restored_path)
    restored_hash = sha256_file(restored_path)

    response = send_file(
        restored_path,
        as_attachment=True,
        download_name=restored_name
    )

    response.headers["X-Restored-Name"] = restored_name
    response.headers["X-Restored-SHA256"] = restored_hash

    return response


if __name__ == "__main__":
    print("Starting Nexus Compressor Web App...")
    print("Open: http://127.0.0.1:5050")

    app.run(
        host="127.0.0.1",
        port=5051,
        debug=True
    )
