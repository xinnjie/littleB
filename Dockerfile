FROM ubuntu:18.04
COPY /minigame_server_install/bin/minigame_server /apps
COPY scripts/ubuntu_setup.sh /scripts
RUN sudo bash /scripts/ubuntu_setup.sh
EXPOSE 10002
CMD ["/apps/minigame_server"]
