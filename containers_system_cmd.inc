
CMD:cont(playerid)
{
	/*

		ваши проверки на авторизованность, админку и т. д

	*/
	return ContSystem:ShowContMenu(playerid);
}

CMD:freezecont(playerid)
{
	/*

		ваши проверки на авторизованность, админку и т. д

	*/
	containers_freezed = !containers_freezed;

	SendClientMessageEx(playerid, -1, " "cl_orange"Вы "cl_white"%s "cl_orange"аукцион за контейнеры", (containers_freezed) ? ("заморозили") : ("разморозили"));
	return true;
}