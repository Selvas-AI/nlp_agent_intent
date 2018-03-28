FROM frolvlad/alpine-python3:latest

MAINTAINER SelvasAI.MLLab <SelvasAI.MLLab@selvas.com>

ENV DEBIAN_FRONTEND noninteractive

ADD env /env

RUN apk add --no-cache tini tzdata && \
	ln -sf /usr/share/zoneinfo/Asia/Seoul /etc/localtime && \
	pip3 install -r /env/requirements.txt

ADD src /src

EXPOSE 8443

ENTRYPOINT ["/sbin/tini", "--"]

CMD [ "python3", "main.py" ]
