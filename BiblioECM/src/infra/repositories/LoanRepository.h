#pragma once

#include "../../core/entities/Loan.h"
#include "../db/database.h"

class LoanRepository {
private:
    Database& db;
public:
    explicit LoanRepository(Database& db);
    ECM::Result salvar(const Loan& u);
    ECM::Expected<LoanState> getLoanState(const UserId& user, const ItemId& item);
    ECM::Result temEmprestimo(const UserId&, const BookId&);
    ECM::Result realizarDevolucao(int loanId);
    ECM::Result renovar(int loanId, int limit);
    ECM::Expected<std::vector<std::chrono::year_month_day>> datasPrevistasPorLivro(const BookId& bookId);
    ECM::Expected<int> ativosPorUser(const UserId& id);

};



