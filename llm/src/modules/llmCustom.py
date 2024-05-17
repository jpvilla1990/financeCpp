import os
from onprem import LLM
from sentence_transformers import SentenceTransformer, util
import numpy
from torch import Tensor
from modules.fileSystem import FileSystem
from modules.documentsHandler import DocumentsHandler
from modules.timeManager import TimeManager
from modules.utils import Utils
import config

class LlmCustsom(FileSystem, DocumentsHandler):
    """
    Class to customize LLM from onprem
    """
    def __init__(self):
        super(LlmCustsom, self).__init__()
        self.__llm : LLM = LLM(use_larger=True, n_gpu_layers=35, confirm=False)
        self.__documents : str = self._getPaths()["documents"]

    def __getEmbeddingModel(self) -> SentenceTransformer:
        """
        Private method to get an embedding model
        """
        model = SentenceTransformer(config.models["sentenceModel"])
        return model

    def __computeSimilarity(self, sentence1, sentence2) -> float:
        """
        Private method to calculate similarity between two sentences
        """
        model : SentenceTransformer = self.__getEmbeddingModel()
        embedding1 : Tensor = model.encode(sentence1, convert_to_tensor=True)
        embedding2 : Tensor = model.encode(sentence2, convert_to_tensor=True)
        cosine_score : Tensor = util.pytorch_cos_sim(embedding1, embedding2)
        return cosine_score.cpu().numpy()[0][0]
    
    def __saveNewDocuments(self):
        """
        Private method to store new documents as files
        """
        documents : list = self.getDocuments()
        numberDocuments : int = len(documents)

        for documentIndex in range(numberDocuments):
            documentPath : str = os.path.join(self._getPaths()["documents"], str(documentIndex))
            documentFile : str = os.path.join(documentPath, config.llmConfig["documentName"] + str(documentIndex) + config.llmConfig["documentFormat"])
            self._createFolder(documentPath)
            self._saveFile(documentFile, documents[documentIndex]["documents"])

    def __getData(self) -> dict:
        """
        Static method to get data dictionary
        """
        dataFilePath : str = self._getFiles()["data"]
        if self._checkFileExists(dataFilePath):
            return Utils.readYaml(dataFilePath)
        else:
            return dict()
        
    def __updateData(self, elementPair : tuple):
        """
        Private method to save new data
        """
        dataFilePath : str = self._getFiles()["data"]
        data : dict = self.__getData()

        data[elementPair[0]] = elementPair[1]
        Utils.writeYaml(dataFilePath, data)

    def __queryNewDocuments(self) -> bool:
        """
        Private method to check if new documents have to be queried
        """
        currentDate : str = TimeManager.getCurrentDate()

        data : dict = self.__getData()

        if "currentDate" in data:
            lastDate : str = data["currentDate"]
            return not TimeManager.dateEqual(currentDate, lastDate)
        else:
            return True

    def __getNewDocuments(self, stock : str):
        """
        Private method to query new documents and update them
        """
        if self.__queryNewDocuments():
            self.getNews(stock)
            self.__updateData(("currentDate", TimeManager.getCurrentDate()))
            self.__saveNewDocuments()
            self.ingestData()

    def __updateSources(self, response : dict) -> dict:
        """
        Private method to update the source to its url
        """
        indexSource : int = 0
        for source in response["sources"]:
            sourcePath : str = source["source"]
            documentIndex : int
            try:
                documentIndex  = int(
                    sourcePath.split("/")[-2]
                )
            except:
                continue
            documents : dict = self.getDocuments()
            url : str = documents[documentIndex]["url"]

            response["sources"][indexSource]["source"] = url

            indexSource += 1

        return response

    def ingestData(self):
        """
        Public method to ingest new data
        """
        self.__llm.ingest(self.__documents)

    def ask(self, question : str) -> dict:
        """
        Public method to perform a prompt and return the answer
        """
        response : dict = dict()
        result : any = self.__llm.ask(question)

        responseString : str = ""
        sources : list = list()

        answer : str = result["answer"]
        documents : list = result["source_documents"]

        uniqueSources : set = set()

        for doc in documents:
            answerScore = self.__computeSimilarity(answer, doc.page_content)
            questionScore = self.__computeSimilarity(question, doc.page_content)
            if answerScore < config.score["answer"] or questionScore < config.score["question"]:
                continue
            uniqueSources.add(
                (
                    doc.metadata["source"],
                    doc.metadata.get("page", None),
                    doc.page_content,
                    questionScore,
                    answerScore,
                )
            )

        uniqueSourcesList : list = list(uniqueSources)
        uniqueSourcesList.sort(key=lambda tup: tup[-1], reverse=True)

        if uniqueSourcesList:
            for source in uniqueSourcesList:
                source : str = str(source[0])
                pageSource : int = int(source[1]) + 1 if isinstance(source[1], int) else 1
                contentSource : str = str(source[2])
                questionScoreSource : str = str(source[3])
                answerScoreSource : str = str(source[4])

                sources.append(
                    {
                        "source" : source,
                        "page" : pageSource,
                        "content" : contentSource,
                        "questionScore" : questionScoreSource,
                        "answerScore" : answerScoreSource,
                    }
                )

            responseString = str(answer)

        else:
            responseString = config.responses["noAnswer"]

        response.update({
            "response" : responseString,
            "sources" : sources,
        })

        return response

    def getStockStatus(self, stock : str):
        """
        Public method to get the stock status
        """
        self.__getNewDocuments(stock)
        response : dict = self.ask(config.llmQuestions["statusStock"] + " " + stock)
        response = self.__updateSources(response)

        return response