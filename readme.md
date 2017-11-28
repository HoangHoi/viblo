# <p align="center">Aquarium server</p>

## Install Laravel server

### config file .env

```
DB_CONNECTION=mysql
DB_HOST=127.0.0.1
DB_PORT=3306
DB_DATABASE=homestead
DB_USERNAME=homestead
DB_PASSWORD=secret
```

### run command

```bash
composer install
php artisan migrate
chmod 777 -R storage
chmod 777 -R bootstrap/cache
php artisan key:generate
```
