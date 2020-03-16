import requests
from bs4 import BeautifulSoup
from urllib.parse import urlparse

def status_decode(obj, params) :
	if not params :
		json_request = requests.get(obj)
	else :
		json_request = requests.get(obj, params)
	status = json_request.status_code
	status_message = {
		200 : "SUCCESS",
		400 : "ERROR: Bad request",
		401 : "ERROR: Not authenticated",
		404 : "ERROR: not found"
	}
	print(status_message.get(status))

	return json_request.json()['results']

def html_extract_review(url) :
	page = requests.get(url)
	soup = BeautifulSoup(page.text, "lxml")
	content = soup.find_all("p", attrs = {"class":"css-exrw3m evys1bk0"})
	
	summary = ""
	for c in content :
		summary = summary + c.text
	return summary

def html_extract_source(url) :
	#Parse the most read articles page to find the most read article
	page = requests.get(url)
	soup = BeautifulSoup(page.text, "lxml")
	content = soup.find_all("div", attrs = {"id":"mostread"})
	#content = soup.find_all("div", attrs = {"id":"tab_multimedia_data"})

	link = ""
	i = 0
	while link == "" :
		first_article = content[0].find_all("a")
		if i < len(first_article) :
			link = first_article[i].get("href")
			o = urlparse(link)
			if "/Video" in o.path:
				print("*" * 75)
				print("ERROR: This link is a video %s" % (link))
				print("*" * 75)
				link = ""
				i = i + 1
		else :
			return 0
	
	#Parse through the most read article
	article = requests.get(link)
	soup = BeautifulSoup(article.text, "lxml")

	title_div = soup.find_all("div", attrs = {"class":"article_details"})
	title = title_div[0].find_all("h1")

	content = soup.find_all("article", attrs = {"id":"article-body"})
	final_text = ""
	for element in content :
		text = element.find_all("p")
		for t in text :
			final_text = final_text + t.text
	return title[0].text, final_text