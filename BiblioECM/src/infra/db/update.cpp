#include "update.h"

ECM::Result updateDb(Database& db) {
	auto update_loans = db.exec(R"(
        UPDATE loans SET status = 2 WHERE status = 1 AND due_at < strftime('%Y%m%d', 'now');
    )");

	if (!update_loans) {
		return update_loans;
	}

	auto update_membros = db.exec(R"(
        UPDATE membros SET status = 2 WHERE id IN (SELECT user_id FROM loans WHERE status = 2)
    )");

	if (!update_membros) {
		return update_membros;
	}

	/* Lembrar de atualizar a tabela de itens caso decidir que ela deve guardar o status */
	
	   
	return {};
}
