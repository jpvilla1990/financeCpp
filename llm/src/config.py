paths : dict = {
    "data" : ["data"],
    "documents" : ["data", "documents"],
}

files : dict = {
    "data" : ["data", "data.yml"],
}

timeConfig : dict = {
    "dateFormat" :  "YYYY-mm-dd",
}

models : dict = {
    "sentenceModel" : "all-MiniLM-L6-v2",
}

llmQuestions : dict = {
    "statusStock" : "give me a summary of the current status of the company",
}

llmConfig : dict = {
    "documentName" : "documentSource",
    "documentFormat" : ".txt",
}

score : dict = {
    "answer" : 0.5,
    "question" : 0.3,
}

responses : dict = {
    "noAnswer" : "There is not answer to the given question",
}

request : dict = {
    "headers" : {
        'User-Agent' : 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.36',
    },
    "searchQuery" : " news today english",
    "timeOut" : 30,
}

characters : dict = {
    "doubleQuotes" : 34,
    "<" : 60,
    ">" : 62,
}

filterRules : dict = {
    "spaces" : 10,
    "maxLenWord" : 15,
    "maxPortionNumbers" : 0.3,
    "maxPortionNumberWordsLenOne" : 0.1,
    "maxNumberDocuments" : 10,
}