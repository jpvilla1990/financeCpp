# How to run the rapidAPI request with curl
curl --request GET   --url 'https://yahoo-finance15.p.rapidapi.com/api/v1/markets/stock/modules?ticker=MGB.DE&module=financial-data'    --header 'X-RapidAPI-Host: yahoo-finance15.p.rapidapi.com'      --header 'X-RapidAPI-Key: xx'

# how to load a docker image
docker login
bash dockerBuild.sh

# Run docker container mysql
docker run -d --name project-mysql-local -e MYSQL_ROOT_PASSWORD=admin -p 3306:3306 -p 33060:33060 mysql:8.1.0
# Run docker container pytorch
docker run -it --rm -v /home/jpvilla1990/financeCpp/llm/data:/workspace/data -p 8300:8000 jpvilla1990/llm:0.2.0

# Get documents
from googlesearch import search
for element in search("baywa news")
    print(element)
then query each url and save the documents from there