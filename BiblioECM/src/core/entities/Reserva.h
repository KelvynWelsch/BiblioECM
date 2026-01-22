#pragma once
#include <chrono>
#include "../ids/UserId.h"
#include "../ids/BookId.h"

enum class ReservaStatus {
    Ativa = 1,
    Atendida = 0,
    Cancelada = -1,
    AguardandoRetirada = 2
};

class Reserva {
public:
    Reserva(UserId user,
        BookId book,
        std::chrono::year_month_day data) : user(user), book(book), data(data) {
        id = 0;
        status = ReservaStatus::Ativa;
    };

    const int getId() const { return id; };
    const UserId& getUser() const { return user; };
    const BookId& getBook() const { return book; };
    ReservaStatus getStatus() const { return status; };
    std::chrono::year_month_day getData() const { return data; };

    //void cancelar();
    //void atender();

    static ReservaStatus parseReservaStatus(int s) {
        switch (s) {
        case 0: return ReservaStatus::Atendida;
        case 2: return ReservaStatus::Cancelada;
        case 1: return ReservaStatus::Ativa;
        }
    }

private:
    int id;
    UserId user;
    BookId book;
    std::chrono::year_month_day data;
    ReservaStatus status;
};


