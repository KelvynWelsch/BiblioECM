#pragma once

#include "Item.h"
#include "User.h"
#include "../states/LoanState.h"
#include <string>
#include <chrono>

class Loan {
public:
    int id;
    ItemId item_id;
    UserId user_id;
    LoanStatus status;

    std::chrono::year_month_day loaned_at;
    std::chrono::year_month_day due_at;
    std::optional<std::chrono::year_month_day> returned_at; 
    int n_Renov = 0;

    Loan() = default;

    
    Loan(int id, const ItemId& item_id, const UserId& user_id,
        const std::chrono::year_month_day& loaned,
        const std::chrono::year_month_day& due)
        : id(id), item_id(item_id), user_id(user_id), status(LoanStatus::Ativo),
        loaned_at(loaned), due_at(due) {
    }

    std::string get_Id() const {
        return std::to_string(id);
    }

    static std::string loanToStatus(LoanStatus s) {
        switch (s) {
        case LoanStatus::Ativo: return "Ativo";
        case LoanStatus::Atrasado: return "Atrasado";
        case LoanStatus::Devolvido: return "Devolvido";
        }
    }

    static LoanStatus parseLoanStatus(int s) {
        switch (s) {
        case 1: return LoanStatus::Ativo;
        case 2: return LoanStatus::Atrasado;
        default: return LoanStatus::Devolvido;
        }
    }
};

