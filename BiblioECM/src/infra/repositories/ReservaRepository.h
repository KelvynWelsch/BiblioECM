#pragma once

#include "../../core/dto/ReservaInfo.h"
#include "../../core/entities/Reserva.h"
#include "../db/database.h"


class ReservaRepository {
private:
	Database& db;
public:
	ReservaRepository(Database& db);

	ECM::Result inserir(const Reserva&);

	ECM::Result usuarioJaReservou(const UserId&, const BookId&);

	ECM::Expected<int> ativasPorLivro(const BookId& book);

	ECM::Expected<ReservaInfo> proximaReserva(const BookId&);

	ECM::Result updateStatus(int id, ReservaStatus status);

	ECM::Expected<std::vector<ReservaInfo>> listarAtivasPorUser(const UserId&);

	ECM::Expected<int> ativasPorUser(const UserId& user_id);

	//ECM::Expected<std::vector<Reserva>> listarAtivasPorLivro(const BookId&);


};

