# 🧠 Pipeline Runner

A real-time, multithreaded signal processing CLI pipeline built in modern C++17.

Supports configurable DAG-style processing pipelines with streaming input (e.g., sine wave), FFT, filtering, and multiple output sinks (e.g., file logger, Graphviz export, metrics, etc.).

---

## 🚀 Features

- ✅ Multithreaded pipeline execution
- ✅ DAG-based configurable dataflow (via JSON)
- ✅ Real-time sine wave input generator
- ✅ FFT processing using FFTW
- ✅ Filtering stages with thresholds
- ✅ Output to files (`output.txt`, `debug.log`)
- ✅ Logging with `spdlog`
- ✅ Monitoring with metrics (`metrics.json`)
- ✅ Graphviz `.dot` output for visualization
- ✅ Full containerization using Docker

---

## 🐳 Run with Docker (No Setup Needed)

### 🔧 1. Build the Docker Image

```bash
docker build -t pipeline-runner .
```

### ▶️ 2. Run the App

```bash
docker run --rm pipeline-runner
```

### 💾 3. Save Outputs to Your Machine


```bash
docker run --rm -v $(pwd)/build/output:/app/output pipeline-runner
```

📁 Outputs will appear in:

```
build/output/
├── output.txt            # Filtered signal
├── debug.log             # Second sink
├── pipeline.log          # Execution logs
├── metrics.json          # Metrics for monitoring
└── pipeline_graph.dot    # DAG Graphviz visualization
```

To visualize the graph:

```bash
dot -Tpng build/output/pipeline_graph.dot -o build/output/graph.png
xdg-open build/output/graph.png
```

---

## 🧪 Example Config

**`configs/pipeline.json`**

```json
{
  "start_node": "fft",
  "input": {
    "frequency": 2.0,
    "amplitude": 1.0,
    "rate": 100
  },
  "pipeline": [
    { "name": "fft", "type": "FFT", "next": ["filter1", "filter2"] },
    { "name": "filter1", "type": "Filter", "min": 0.1, "max": 1.0, "next": ["file_sink"] },
    { "name": "filter2", "type": "Filter", "min": 0.0, "max": 0.5, "next": ["logger_sink"] },
    { "name": "file_sink", "type": "FileSink", "path": "output.txt" },
    { "name": "logger_sink", "type": "FileSink", "path": "debug.log" }
  ]
}
```

---

## 🧰 Building Locally (Without Docker)

### 🛠️ Dependencies

```bash
sudo apt install build-essential cmake libfftw3-dev libspdlog-dev libfmt-dev nlohmann-json3-dev
```

### 🔨 Build & Run

```bash
mkdir build && cd build
cmake ..
make -j4
./pipeline_runner --config ../configs/pipeline.json
```

---

## 📈 Monitoring (Metrics)

Metrics are exported to `build/output/metrics.json` every few seconds:

```json
{
  "input_count": 52,
  "processed_count": 52,
  "output_count": 5200
}
```

---

## 🗺️ Project Structure

```
pipeline_runner/
├── include/                 # C++ headers (stages, engine, etc.)
├── src/                     # C++ source files
├── configs/pipeline.json    # Configurable DAG pipeline
├── build/output/            # Runtime output
├── Dockerfile               # Container build script
├── CMakeLists.txt
└── README.md
```

---

## 📜 License
MIT or your license of choice.

---

**Developer:** Prachi Tiwari
