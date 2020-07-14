FROM python:3.6-slim

RUN apt-get clean \
    && apt-get -y update

RUN apt-get -y install nginx \
    && apt-get -y install python3-dev \
    && apt-get -y install build-essential

COPY ./config/* /srv/flask_app/

WORKDIR /srv/flask_app

RUN pip3 install -r requirements.txt --src /usr/local/src

COPY ./config/nginx.conf /etc/nginx

COPY ./src/server.py /srv/flask_app/server.py

RUN chmod +x ./start.sh
CMD ["./start.sh"]
