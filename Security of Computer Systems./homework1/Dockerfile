FROM ubuntu:latest

WORKDIR /code

COPY . .

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get install sudo -y
RUN apt-get install acl
RUN apt update && apt install openssh-server -y
RUN service ssh start

RUN ./scripts/init.sh
RUN touch /etc/ssh/sshrc && printf "#!/bin/bash \n exec </dev/tty sudo /code/employee/employee" > /etc/ssh/sshrc

# Python
ENV PYTHONDONTWRITEBYTECODE 1
ENV PYTHONUNBUFFERED 1

RUN apt-get install -y python3.8
RUN apt-get install -y python3-pip
RUN pip3 install -r webapp/requirements.txt

EXPOSE 22

CMD ["./scripts/entrypoint.sh"]
