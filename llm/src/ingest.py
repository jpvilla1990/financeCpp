import os
from pathlib import Path
from onprem import LLM

rootPath : str = Path(os.path.abspath(__file__)).parents[1]
documentsPath : str = os.path.join(rootPath, "data", "documents")
os.makedirs(documentsPath, exist_ok=True)

llm = LLM(use_larger=True, n_gpu_layers=35, confirm=False)
llm.ingest(documentsPath)