
CMD:cont(playerid)
{
	/*

		���� �������� �� ����������������, ������� � �. �

	*/
	return ContSystem:ShowContMenu(playerid);
}

CMD:freezecont(playerid)
{
	/*

		���� �������� �� ����������������, ������� � �. �

	*/
	containers_freezed = !containers_freezed;

	SendClientMessageEx(playerid, -1, " "cl_orange"�� "cl_white"%s "cl_orange"������� �� ����������", (containers_freezed) ? ("����������") : ("�����������"));
	return true;
}