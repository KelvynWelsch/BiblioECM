#pragma once

#include<optional>

enum class LoanStatus {
    Ativo = 1,
    Devolvido = 0,
    Atrasado = 2
};

struct LoanState {
    int loan_id;
    LoanStatus status;
    std::chrono::year_month_day due_at;
    int n_Renov;
    std::optional<std::chrono::year_month_day> returned_at;
};

