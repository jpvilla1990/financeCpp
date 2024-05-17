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

    def ask(self, question : str) -> dict:
        """
        Method to ask questions
        """
        response : dict = self.__llmCustom.ask(question)
        return response
    
    def getStockStatus(self, stock : str) -> dict:
        """
        Method to get stock information
        """
        response : dict = self.__llmCustom.getStockStatus(stock)
        return response

app : FastAPI = FastAPI()
api : LlmApi = LlmApi()

@app.get("/api/ingest")
def getIngest():
    api.ingest()
    return {"message": "Ingestion Documents Completed"}

@app.get("/api/ask")
def getAsk(question : str):
    response : dict = api.ask(question)
    return {"message": response}

@app.get("/api/stockStatus")
def getStockStatus(stock : str):
    response : dict = api.getStockStatus(stock)
    return {"message" : response}