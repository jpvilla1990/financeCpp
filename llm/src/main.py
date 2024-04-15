from modules.llmCustom import LlmCustsom
from fastapi import FastAPI

class LlmApi(object):
    """
    Class to handle API llm
    """
    def __init__(self) -> None:
        self.__llmCustom : LlmCustsom = LlmCustsom()

    def ingest(self):
        """
        Method to ingest documents
        """
        self.__llmCustom.ingestData()

app : FastAPI = FastAPI()
api : LlmApi = LlmApi()

@app.get("/api/ingest")
def get_path1():
    api.ingest()
    return {"message": "Ingestion Documents Completed"}

@app.get("/api/prompt")
def get_path2():
    return {"message": "prompt"}