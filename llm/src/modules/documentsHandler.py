import urllib
import json
import urllib.request
from googlesearch import search
import config

class DocumentsHandler(object):
    """
    Class to execute google scraping commands
    """
    def __init__(self):
        self.__documents : dict = dict()

    def __filterString(self, string : str):
        """
        Privat method to filter string to evaluate it is a meaninful document
        """
        filteredString : str = string
        if " " not in string:
            return ""
        
        stringsList : list = filteredString.split(" ")
        filteredStringsList : list = list()

        wordsLenOne : int = 0
        for string in stringsList:
            if string != "" and len(string) < config.filterRules["maxLenWord"]:
                if len(string) == 1:
                    wordsLenOne += 1
                filteredStringsList.append(string)

        if len(filteredStringsList) != 0:
            if (wordsLenOne / len(filteredStringsList)) > config.filterRules["maxPortionNumberWordsLenOne"]:
                return ""

        filteredString = " ".join(filteredStringsList)

        if len(filteredStringsList) < config.filterRules["spaces"]:
            return ""

        numNumbers : int = 0
        for character in filteredString:
            if character.isnumeric():
                numNumbers += 1

        if numNumbers / len(filteredString) > config.filterRules["maxPortionNumbers"]:
            return ""
        
        filteredString = filteredString.replace("\n", "").replace("\\", "")

        return filteredString

    def __sortByLenghtDocument(self, strings : list) -> list:
        """
        Private method to sort a document by the lenght of its strings
        """
        orderedStrings : list = list()
        sizeList : list = [len(string) for string in strings]
        indexedList = list(enumerate(sizeList))
        sortedIndexedList = sorted(indexedList, key=lambda x: x[1])

        reorderedIndices = [index for index, _ in sortedIndexedList]

        for index in reorderedIndices:
            orderedStrings.append(strings[index])

        return orderedStrings

    def __processDocument(self, document : bytes) -> list:
        """
        Private method to process document
        """
        strings : list = list()
        string : str = ""
        fillString : bool = False
        ignoreTag : bool = False
        for character in document:
            if character == config.characters["doubleQuotes"]:
                if fillString:
                    fillString = False
                    filteredString : str = self.__filterString(string)
                    if filteredString != "":
                        strings.append(filteredString + "\n")
                    string = ""
                else:
                    fillString = True
            else:
                if fillString:
                    if character == config.characters["<"]:
                        ignoreTag = True
                    elif character == config.characters[">"]:
                        ignoreTag = False
                    else:
                        if ignoreTag is False:
                            string += chr(character)

        strings = self.__sortByLenghtDocument(strings)

        if len(strings) > config.filterRules["maxNumberDocuments"]:
            return strings[len(strings) - config.filterRules["maxNumberDocuments"] : len(strings)]
        else:
            return strings

    def getDocuments(self) -> list:
        """
        Public method to get list of documents urls
        """
        return self.__documents

    def getNews(self, query : str) -> list:
        """
        Public method to query urls from a google
        """
        query += config.request["searchQuery"]
        self.__documents : dict = dict()
        documentsIndex : int = 0
        for documentUrl in search(query):
            req : urllib.request.Request = urllib.request.Request(documentUrl, headers=config.request["headers"])
            try:
                with urllib.request.urlopen(req, timeout=config.request["timeOut"]) as page:
                    document : str = page.read()
                    self.__documents.update({
                        documentsIndex : {
                            "documents" : TimeManager.getCurrentDate() + " " + self.__processDocument(document),
                            "url" : documentUrl,
                        }
                    })
                    documentsIndex += 1
            except:
                # TODO: log link which failed to be opened
                continue
