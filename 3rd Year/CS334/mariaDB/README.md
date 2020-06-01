# MySQL Sandbox Dataset

## Installation

1) Install Docker for [Ubuntu](https://docs.docker.com/engine/install/ubuntu/) (or other distros) or [Windows](https://docs.docker.com/docker-for-windows/install/). Windows 10 Pro required.
2) Complete the [post-installation steps](https://docs.docker.com/compose/install/).
3) Install [Docker Compose](https://docs.docker.com/compose/install/).
4) You may need to log out / restart PC or run Docker commands with `sudo`/admin priviledges.

## Running the database

With Docker and Docker Compose installed, run the following command in this directory:
```
docker-compose up
```
This will start MariaDB, initialize the database with [db_setup.sql](db_setup.sql) and run Adminer.

You can connect to MariaDB on port 3306 and to Adminer at `http://localhost:8080`. Log into Adminer with the following credentials:

System: MySQL
Server: mariadb
User: user
Password: password
Database: college_db

## Configurations

Any environment variables can be edited or files mounted via the `docker-compose.yml` file.
