from onprem import LLM
from sentence_transformers import SentenceTransformer, util
import numpy
from torch import Tensor
from modules.fileSystem import FileSystem
import config

class LlmCustsom(FileSystem):
    """
    Class to customize LLM from onprem
    """
    def __init__(self):
        super().__init__()
        self.__llm : LLM = LLM(use_larger=True, n_gpu_layers=35, confirm=False)
        self.__documents : str = self.getPaths()["documents"]

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
                pageSource : int = int(source[1]) + 1 if isinstance(source[1], int) else int(source[1])
                contentSource : str = str(source[2])
                questionScoreSource : float = float(source[3])
                answerScoreSource : float = float(source[4])

                sources.append(
                    {
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