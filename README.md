# Specyfikacja projektu – Game of Life

---

## 1. Opis projektu

Projekt stanowi interaktywną implementacje **Conway's Game of Life** w języku C, z graficznym interfejsem opartym na bibliotece **SDL3**. Aplikacja umożliwia użytkownikowi ręczne rysowanie stanu początkowego, uruchamianie i pauzowanie symulacji, regulację jej prędkości oraz zapis i odczyt stanów gry z pliku.

---

## 2. Cel i zakres projektu

### 2.1 Cel
Celem projektu jest stworzenie w pełni funkcjonalnej, graficznej aplikacji interaktywnej demonstrującej znajomość niskopoziomowego programowania w C — w szczególności: modularności kodu, dynamicznego zarządzania pamięcią, wskaźników, struktur danych, operacji na plikach oraz obsługi błędów.

### 2.2 Zakres
- Implementacja logiki (zasady Conwaya).
- Graficzny interfejs użytkownika (SDL3) z obsługą myszy i klawiatury.
- Modularny podział kodu na `main.c`, `logic.c`, `ui.c`, `io.c`.
- Dynamicznie alokowana siatka komórek przechowywana w strukturze `Grid`.
- Zapis i odczyt stanu planszy z pliku tekstowego.
- Zapis logów zdarzeń aplikacji do pliku.
- Kompleksowa obsługa błędów i walidacja danych wejściowych.

## 3. Wymagania funkcjonalne

### 3.1 Symulacja

- Aplikacja wyświetla siatkę komórek w oknie graficznym.
- Użytkownik może rysować żywe komórki `lewym przyciskiem myszy`.
- Użytkownik może usuwać komórki `prawym przyciskiem myszy`.
- Klawisz `SPACJA` uruchamia i pauzuje symulację.
- Klawisz `BACKSPACE` czyści całą planszę.
- Klawisze `↑` / `↓` regulują prędkość symulacji.
- Klawisz `ESCAPE` zamyka aplikację.

### 3.2 Operacje na plikach

- Klawisz `S` zapisuje bieżący stan planszy do pliku w katalogu `saves/`.
- Klawisz `L` wczytuje stan planszy z wybranego pliku w katalogu `saves/`.
- Aplikacja zapisuje log zdarzeń (uruchomienie, zapis, wczytanie, błędy) do katalogu `logs/`.

### 3.3 Konfiguracja

- Rozmiar okna, rozmiar komórki, domyślna prędkość symulacji i kolory są definiowane w `include/config.h`.

## 4. Wymagania niefunkcjonalne

- Kod podzielony na moduły: `main.c`, `logic.c`, `ui.c`, `io.c`.
- Wszystkie funkcje posiadają czytelne nazwy i komentarze.
- Kompilacja bez ostrzeżeń przy flagach `-Wall -Wextra`.
- Brak wycieków pamięci (weryfikacja narzędziem valgrind).
- Siatka przechowywana jako dynamicznie alokowana struktura `Grid` (`malloc`/`free`).
- Obsługa błędów przy operacjach plikowych (brak pliku, błąd zapisu, przekroczenie rozmiaru).

## 5. Architektura modułów

```
game_of_life/
├── include/
│   ├── config.h       # Stałe konfiguracyjne (rozmiar, kolory, prędkość)
│   ├── logic.h        # Interfejs logiki symulacji i struktury Grid
│   ├── ui.h           # Interfejs rysowania i obsługi wejścia
│   └── io.h           # Interfejs zapisu/odczytu plików i logowania
├── src/
│   ├── main.c         # Punkt wejścia, pętla główna
│   ├── logic.c        # Reguły Conwaya, alokacja siatki
│   ├── ui.c           # Rysowanie SDL, obsługa zdarzeń
│   └── io.c           # Zapis/odczyt stanu, logowanie
├── saves/             # Pliki zapisanych stanów gry
├── logs/              # Logi aplikacji
├── docs/              # Dokumentacja projektu
├── tests/             # Scenariusze testowe
└── Makefile
```

## Kluczowa struktura danych

typedef struct {
    int**  cells;
    int    rows;
    int    cols;
} Grid;

## 6. Format pliku zapisu stanu

Plik tekstowy `.gol` w katalogu `saves/`, jeden wiersz na rząd siatki, komórki kodowane jako `0` (martwa) lub `1` (żywa):

```
0001000000...
0010100000...
...
```

Pierwsza linia pliku zawiera nagłówek z rozmiarem siatki:
**Aplikacja waliduje poprawność pliku i odrzuca niepoprawne dane**
```
ROWS=45 COLS=85
0001000...
```

## 7. Format logów

Plik `logs/app.log`, wpisy chronologiczne:

```
[2026-04-17 14:32:01] INFO  Aplikacja uruchomiona
[2026-04-17 14:32:45] INFO  Stan zapisany: saves/stan_001.gol
[2026-04-17 14:33:10] ERROR Nie można otworzyć pliku: saves/brak.gol
```

## 8. Scenariusze testowe

- Uruchomienie aplikacji | Okno SDL otwiera się, siatka pusta
- Rysowanie komórek myszą | Komórki pojawiają się na siatce
- Start symulacji (`SPACJA`) | Plansza ewoluuje zgodnie z regułami
- Zapis stanu (`S`) | Plik pojawia się w `saves/`
- Wczytanie stanu (`L`) | Plansza odtwarza zapisany wzór
- Wczytanie nieistniejącego pliku | Komunikat błędu, aplikacja nie zawiesza się
- Czyszczenie planszy (`BACKSPACE`) | Wszystkie komórki stają się martwe
- Zamknięcie aplikacji (`ESCAPE`) | Pamięć zwolniona, brak wycieków (valgrind)


## 9. Narzędzia i technologie

| Narzędzie | Zastosowanie |
|---|---|
| C (standard C11) | Język implementacji |
| SDL3 | Graficzny interfejs użytkownika |
| GCC | Kompilator |
| GNU Make | System budowania |
| Git / GitHub | Kontrola wersji, zarządzanie zadaniami (Issues) |
| Valgrind | Wykrywanie wycieków pamięci |
