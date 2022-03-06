# BSK
Bezpieczeństwo Systemów Komputerowych MIMUM 2021/2022

# Zadanie 1
Pliki w folderze `employee` zawierają kod źródłowy oraz program wykonywalny aplikacji pracownika.

Pliki w folderze `webapp` zawierają kod źródłowy aplikacji internetowej dla klienta.

Plik w folderze `files` to dostarczony plik z danymi klientów i pracowników.

Pliki w folderze `scripts` to skrypty służące do inicjalizacji maszyny.

## Instrukcja
Obraz kontenera należy zbudować w głównym folderze komendą `docker build -t bsk .`

Kontener należy uruchomić komendą ` docker run --rm -it --name bsk_app -p 8080:8080 -p 22:22 bsk `

Aby połączyć się przez ssh do aplikacji pracownika należy najpierw zdobyć adres ip kontenera używając komendy
```
docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' bsk_app
```
Następnie należy połączyć się przez ssh (np. dla loginu `pracownik` i ip `172.17.0.2`):
```
ssh pracownik@172.17.0.2
```

Aby wejść w aplikację klienta należy udać się w przeglądarce na adres: `https://localhost:8080/login` i się zalogować.
