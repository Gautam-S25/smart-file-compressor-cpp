const $ = id => document.getElementById(id);

const fileInput = $("fileInput");
const dropZone = $("dropZone");
const fileName = $("fileName");

const compressBtn = $("compressBtn");
const decompressBtn = $("decompressBtn");

const originalSize = $("originalSize");
const outputSize = $("outputSize");
const statusText = $("statusText");
const executionTime = $("executionTime");

const logBox = $("logBox");
const loader = $("loader");
const loaderText = $("loaderText");
const toast = $("toast");

const previewName = $("previewName");
const previewType = $("previewType");
const previewSize = $("previewSize");
const previewModified = $("previewModified");
const previewContent = $("previewContent");

const reportOperation = $("reportOperation");
const reportFile = $("reportFile");
const reportInputSize = $("reportInputSize");
const reportOutputSize = $("reportOutputSize");
const reportSaved = $("reportSaved");
const reportTime = $("reportTime");
const reportStatus = $("reportStatus");
const integrityStatus = $("integrityStatus");

const uniqueBytes = $("uniqueBytes");
const avgCodeLength = $("avgCodeLength");
const encodedBits = $("encodedBits");
const spaceEfficiency = $("spaceEfficiency");
const compressionRatio = $("compressionRatio");
const fileCategory = $("fileCategory");
const recommendation = $("recommendation");
const frequencyChart = $("frequencyChart");
const huffmanTree = $("huffmanTree");

const historyBody = $("historyBody");

let selectedFile = null;

function formatBytes(bytes) {
    if (!bytes) return "0 B";

    const units = ["B", "KB", "MB", "GB"];
    const index = Math.floor(Math.log(bytes) / Math.log(1024));

    return (
        bytes / Math.pow(1024, index)
    ).toFixed(index === 0 ? 0 : 2) + " " + units[index];
}

function addLog(message) {
    const time = new Date().toLocaleTimeString();

    logBox.textContent += `\n> [${time}] ${message}`;
    logBox.scrollTop = logBox.scrollHeight;
}

function showToast(message, ok = true) {
    toast.textContent = message;
    toast.className = "toast";

    if (!ok) {
        toast.classList.add("error-toast");
    }

    setTimeout(() => {
        toast.classList.add("hidden");
    }, 2800);
}

function setLoading(active, text = "Processing...") {
    loaderText.textContent = text;

    if (active) {
        loader.classList.remove("hidden");
        compressBtn.disabled = true;
        decompressBtn.disabled = true;
    } else {
        loader.classList.add("hidden");
        updateButtons();
    }
}

function updateButtons() {
    if (!selectedFile) {
        compressBtn.disabled = true;
        decompressBtn.disabled = true;
        return;
    }

    const ext = selectedFile.name.split(".").pop().toLowerCase();

    compressBtn.disabled = ext === "huff";
    decompressBtn.disabled = ext !== "huff";
}

document.querySelectorAll(".tab-btn").forEach(button => {
    button.addEventListener("click", () => {
        document
            .querySelectorAll(".tab-btn")
            .forEach(b => b.classList.remove("active"));

        document
            .querySelectorAll(".tab-content")
            .forEach(section => section.classList.remove("active"));

        button.classList.add("active");
        $(button.dataset.tab).classList.add("active");
    });
});

dropZone.addEventListener("click", () => fileInput.click());

dropZone.addEventListener("dragover", event => {
    event.preventDefault();
    dropZone.classList.add("dragging");
});

dropZone.addEventListener("dragleave", () => {
    dropZone.classList.remove("dragging");
});

dropZone.addEventListener("drop", event => {
    event.preventDefault();
    dropZone.classList.remove("dragging");

    if (event.dataTransfer.files.length) {
        selectedFile = event.dataTransfer.files[0];
        loadSelectedFile();
    }
});

fileInput.addEventListener("change", () => {
    if (fileInput.files.length) {
        selectedFile = fileInput.files[0];
        loadSelectedFile();
    }
});

async function loadSelectedFile() {
    const file = selectedFile;

    fileName.textContent = file.name;
    originalSize.textContent = formatBytes(file.size);

    previewName.textContent = file.name;
    previewType.textContent = file.type || "Unknown";
    previewSize.textContent = formatBytes(file.size);
    previewModified.textContent =
        new Date(file.lastModified).toLocaleString();

    reportFile.textContent = file.name;
    reportInputSize.textContent = formatBytes(file.size);

    statusText.textContent = "File Loaded";
    updateButtons();

    const ext = file.name.split(".").pop().toLowerCase();

    fileCategory.textContent =
        ext === "huff"
            ? "Compressed Huffman File"
            : ext.toUpperCase() + " File";

    const textTypes = [
        "txt","csv","json","xml","html",
        "css","js","py","cpp","hpp","md"
    ];

    if (textTypes.includes(ext)) {
        try {
            const text = await file.text();
            previewContent.textContent = text.slice(0, 5000);
        } catch {
            previewContent.textContent = "Unable to preview this file.";
        }
    } else if (ext === "huff") {
        previewContent.textContent =
            "Compressed Huffman file. Use Extract to restore it.";
    } else {
        previewContent.textContent =
            "Preview is not available for this binary/document file type.";
    }

    addLog(`Selected file: ${file.name}`);

    if (ext !== "huff") {
        await runAnalysis(file);
    }
}

async function runAnalysis(file) {
    const data = new FormData();
    data.append("file", file);

    try {
        const response = await fetch("/analyze", {
            method: "POST",
            body: data
        });

        if (!response.ok) return;

        const analysis = await response.json();

        uniqueBytes.textContent = analysis.UNIQUE_BYTES || "0";
        avgCodeLength.textContent =
            (analysis.AVG_CODE_LENGTH || "0") + " bits";

        encodedBits.textContent =
            Number(
                analysis.TOTAL_ENCODED_BITS || 0
            ).toLocaleString() + " bits";

        const rawBits = file.size * 8;
        const encoded = Number(analysis.TOTAL_ENCODED_BITS || 0);

        if (rawBits > 0) {
            const saving = (1 - encoded / rawBits) * 100;

            spaceEfficiency.textContent =
                saving.toFixed(2) + "%";

            compressionRatio.textContent =
                (encoded / rawBits).toFixed(2) + " : 1";
        }

        recommendation.textContent =
            `${analysis.UNIQUE_BYTES || 0} unique bytes • Avg code ${
                analysis.AVG_CODE_LENGTH || 0
            } bits`;

        renderFrequencyChart(
            analysis.TOP_FREQUENCIES || ""
        );

        renderHuffmanTree(
            analysis.CODEBOOK || ""
        );

        renderCodebook(
            analysis.CODEBOOK_DETAILS || ""
        );

        addLog("C++ Huffman analysis completed.");

    } catch (error) {
        addLog(`Analysis error: ${error.message}`);
    }
}

function byteLabel(value) {
    const n = Number(value);

    if (n === 32) return "Space";
    if (n === 10) return "\\n";
    if (n === 9) return "Tab";

    if (n >= 33 && n <= 126) {
        return String.fromCharCode(n);
    }

    return "Byte " + n;
}

function renderFrequencyChart(rawData) {
    frequencyChart.innerHTML = "";

    if (!rawData) {
        frequencyChart.innerHTML =
            '<div class="empty-chart">No frequency data available.</div>';
        return;
    }

    const entries = rawData
        .split(",")
        .map(item => {
            const [byte, count] = item.split(":");
            return {
                byte,
                count: Number(count)
            };
        })
        .filter(x => Number.isFinite(x.count));

    if (!entries.length) {
        frequencyChart.innerHTML =
            '<div class="empty-chart">No frequency data available.</div>';
        return;
    }

    const max = Math.max(...entries.map(x => x.count));

    entries.forEach(item => {
        const wrap = document.createElement("div");
        wrap.className = "frequency-item";

        const area = document.createElement("div");
        area.className = "frequency-bar-area";

        const bar = document.createElement("div");
        bar.className = "frequency-bar";

        bar.style.height =
            Math.max((item.count / max) * 100, 3) + "%";

        const count = document.createElement("div");
        count.className = "frequency-count";
        count.textContent = item.count;

        const label = document.createElement("div");
        label.className = "frequency-label";
        label.textContent = byteLabel(item.byte);

        area.appendChild(bar);
        wrap.append(area, count, label);
        frequencyChart.appendChild(wrap);
    });
}


function renderHuffmanTree(rawCodebook) {
    if (!huffmanTree) return;

    huffmanTree.innerHTML = "";

    if (!rawCodebook) {
        huffmanTree.innerHTML =
            '<div class="empty-chart">No Huffman tree data available.</div>';
        return;
    }

    const root = {};

    const entries = rawCodebook
        .split(",")
        .map(item => {
            const parts = item.split(":");
            return {
                byte: parts[0],
                code: parts.slice(1).join(":")
            };
        })
        .filter(x => x.byte !== undefined && x.code);

    entries.forEach(entry => {
        let node = root;

        for (const bit of entry.code) {
            if (!node[bit]) {
                node[bit] = {};
            }

            node = node[bit];
        }

        node.byte = entry.byte;
        node.code = entry.code;
    });

    function buildNode(node) {
        const wrap =
            document.createElement("div");

        wrap.className =
            "tree-node-wrap";

        const box =
            document.createElement("div");

        box.className =
            "tree-node";

        if (node.byte !== undefined) {
            box.classList.add("leaf");

            box.textContent =
                `${byteLabel(node.byte)} (${node.code})`;
        } else {
            box.textContent = "•";
        }

        wrap.appendChild(box);

        if (node["0"] || node["1"]) {
            const children =
                document.createElement("div");

            children.className =
                "tree-children";

            if (node["0"]) {
                const leftWrap =
                    buildNode(node["0"]);

                const label =
                    document.createElement("span");

                label.className =
                    "tree-edge-label left-edge";

                label.textContent = "0";

                children.appendChild(label);
                children.appendChild(leftWrap);
            }

            if (node["1"]) {
                const rightWrap =
                    buildNode(node["1"]);

                const label =
                    document.createElement("span");

                label.className =
                    "tree-edge-label right-edge";

                label.textContent = "1";

                children.appendChild(label);
                children.appendChild(rightWrap);
            }

            wrap.appendChild(children);
        }

        return wrap;
    }

    const rootContainer =
        document.createElement("div");

    rootContainer.className =
        "tree-root";

    rootContainer.appendChild(
        buildNode(root)
    );

    huffmanTree.appendChild(
        rootContainer
    );
}

function addHistory(
    operation,
    file,
    input,
    output,
    time,
    status
) {
    const row = document.createElement("tr");

    [operation, file, input, output, time, status].forEach(value => {
        const td = document.createElement("td");
        td.textContent = value;
        row.appendChild(td);
    });

    historyBody.prepend(row);
}

async function processFile(endpoint, operation) {
    if (!selectedFile) {
        showToast("Please select a file.", false);
        return;
    }

    const data = new FormData();
    data.append("file", selectedFile);

    const start = performance.now();

    statusText.textContent = "Processing";
    reportStatus.textContent = "Processing";
    integrityStatus.textContent = "Checking...";

    setLoading(true, `${operation} in progress...`);
    addLog(`${operation} started.`);

    try {
        const response = await fetch(endpoint, {
            method: "POST",
            body: data
        });

        const elapsed =
            Math.round(performance.now() - start);

        executionTime.textContent = `${elapsed} ms`;
        reportTime.textContent = `${elapsed} ms`;

        if (!response.ok) {
            let message = "Operation failed";

            try {
                const body = await response.json();
                message = body.error || message;
            } catch {}

            throw new Error(message);
        }

        const blob = await response.blob();

        outputSize.textContent = formatBytes(blob.size);
        reportOutputSize.textContent = formatBytes(blob.size);

        reportOperation.textContent = operation;
        reportStatus.textContent = "Success";
        statusText.textContent = "Success";

        let downloadName =
            operation === "Compression"
                ? selectedFile.name + ".huff"
                : response.headers.get("X-Restored-Name") || "restored_file";

        const originalHash =
            response.headers.get("X-Original-SHA256");

        const restoredHash =
            response.headers.get("X-Restored-SHA256");

        if (operation === "Compression") {
            integrityStatus.textContent =
                originalHash ? "Source hash generated" : "Not available";
        } else {
            integrityStatus.textContent =
                restoredHash ? "Restored hash generated" : "Not available";
        }

        if (operation === "Compression") {
            const saved =
                ((selectedFile.size - blob.size) /
                    selectedFile.size) * 100;

            reportSaved.textContent =
                saved.toFixed(2) + "%";
        } else {
            reportSaved.textContent = "N/A";
        }

        const url = URL.createObjectURL(blob);
        const link = document.createElement("a");

        link.href = url;
        link.download = downloadName;

        document.body.appendChild(link);
        link.click();
        link.remove();

        URL.revokeObjectURL(url);

        addHistory(
            operation,
            selectedFile.name,
            formatBytes(selectedFile.size),
            formatBytes(blob.size),
            `${elapsed} ms`,
            "Success"
        );

        addLog(`${operation} completed successfully.`);
        showToast(`${operation} completed successfully.`);

    } catch (error) {
        statusText.textContent = "Failed";
        reportStatus.textContent = "Failed";
        integrityStatus.textContent = "Not verified";

        addHistory(
            operation,
            selectedFile.name,
            formatBytes(selectedFile.size),
            "-",
            executionTime.textContent,
            "Failed"
        );

        addLog(`${operation} failed: ${error.message}`);
        showToast(error.message, false);

    } finally {
        setLoading(false);
    }
}

compressBtn.addEventListener(
    "click",
    () => processFile("/compress", "Compression")
);

decompressBtn.addEventListener(
    "click",
    () => processFile("/decompress", "Decompression")
);

updateButtons();


// ------------------------------
// Huffman Codebook
// ------------------------------

let currentCodebookRows = [];

function displayByteCharacter(byteValue) {
    const n = Number(byteValue);

    if (n === 32) return "Space";
    if (n === 10) return "\\n";
    if (n === 13) return "\\r";
    if (n === 9) return "Tab";

    if (n >= 33 && n <= 126) {
        return String.fromCharCode(n);
    }

    return `Byte ${n}`;
}


function renderCodebook(rawDetails) {
    const tbody =
        document.getElementById("huffmanCodebook");

    if (!tbody) return;

    tbody.innerHTML = "";
    currentCodebookRows = [];

    if (!rawDetails) {
        tbody.innerHTML = `
            <tr>
                <td colspan="5" class="codebook-empty">
                    No Huffman codebook data available.
                </td>
            </tr>
        `;
        return;
    }

    currentCodebookRows = rawDetails
        .split(";")
        .map(item => {
            const parts = item.split(":");

            return {
                byte: Number(parts[0]),
                frequency: Number(parts[1]),
                code: parts.slice(2).join(":")
            };
        })
        .filter(item =>
            Number.isFinite(item.byte) &&
            Number.isFinite(item.frequency) &&
            item.code
        );

    // Highest-frequency symbols first
    currentCodebookRows.sort(
        (a, b) => b.frequency - a.frequency
    );

    drawCodebookRows(currentCodebookRows);
}


function drawCodebookRows(rows) {
    const tbody =
        document.getElementById("huffmanCodebook");

    if (!tbody) return;

    tbody.innerHTML = "";

    if (!rows.length) {
        tbody.innerHTML = `
            <tr>
                <td colspan="5" class="codebook-empty">
                    No matching codebook entries.
                </td>
            </tr>
        `;
        return;
    }

    rows.forEach(item => {
        const tr = document.createElement("tr");

        const character =
            displayByteCharacter(item.byte);

        tr.innerHTML = `
            <td>${character}</td>
            <td>${item.byte}</td>
            <td>${item.frequency.toLocaleString()}</td>
            <td>
                <span class="code-chip">
                    ${item.code}
                </span>
            </td>
            <td>
                <span class="code-length">
                    ${item.code.length} bits
                </span>
            </td>
        `;

        tbody.appendChild(tr);
    });
}


document
    .getElementById("codebookSearch")
    ?.addEventListener("input", function () {

        const query =
            this.value.trim().toLowerCase();

        if (!query) {
            drawCodebookRows(currentCodebookRows);
            return;
        }

        const filtered =
            currentCodebookRows.filter(item => {

                const character =
                    displayByteCharacter(item.byte)
                    .toLowerCase();

                return (
                    character.includes(query) ||
                    String(item.byte).includes(query) ||
                    item.code.includes(query)
                );
            });

        drawCodebookRows(filtered);
    });
