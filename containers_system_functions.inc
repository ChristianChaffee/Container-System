

// ---- Óçíà¸ò èíäåêñ âðåìåíè â ìàññèâå, êîãäà ó êîíòåéíåðà íà÷í¸òñÿ àóêöèîí
stock ContSystem:GetContStartTimeID(container_id)
{
	new current_hour, current_minute;
	new temp_hour, temp_minute;

	gettime(current_hour, current_minute);

	for(new i; i < sizeof(container_start_time); i++)
	{
		sscanf(container_start_time [i], "p<:>ii", temp_hour, temp_minute);

		if(i == sizeof(container_start_time))
		{
			if(current_hour >= temp_hour) 
			{
				return 0;
			}
		}
		else
		{
			if(current_hour >= temp_hour) 
			{
				continue;
			}
			else 
			{
				return i;
			}
		}
	}
	return 0;
}



// ---- Ðàíäîìàéçåð ïðèçà â êîíòåéíåðå
stock ContSystem:UpdatePrize(container_id, bool:update_class_block = false, bool:update_type_block = false, bool:sql_update = false)
{
	if(!update_class_block)
	{
		SetContainerData(container_id, EC_CLASS, random(CONTAINER_CLASSES_COUNT)+1);
		
		while(GetContainerData(container_id, EC_CLASS) == GetContainerData(container_id, EC_LAST_CLASS))
		{
			SetContainerData(container_id, EC_CLASS, random(CONTAINER_CLASSES_COUNT)+1);
		}
		SetContainerData(container_id, EC_LAST_CLASS, GetContainerData(container_id, EC_CLASS));
	}

	if(!update_type_block)
	{
		SetContainerData(container_id, EC_TYPE, random(CONTAINER_PRIZE_TYPE_COUNT)+1);
		
		while(GetContainerData(container_id, EC_TYPE) == GetContainerData(container_id, EC_LAST_TYPE))
		{
			SetContainerData(container_id, EC_TYPE, random(CONTAINER_PRIZE_TYPE_COUNT)+1);
		}
		SetContainerData(container_id, EC_LAST_TYPE, GetContainerData(container_id, EC_TYPE));
	}

	new prize_successful_list [MAX_PRIZE_BY_TYPES];
	new counter;

	for(new i; i < MAX_CONTAINER_PRIZES; i++)
	{
		if(GetContainerData(container_id, EC_CLASS) == GetPrizeData(i, EC_PRIZE_CLASS))
		{
			if(GetContainerData(container_id, EC_TYPE) == CONTAINER_TYPE_CAR
				&& GetPrizeData(i, EC_PRIZE_TYPE) == CONTAINER_TYPE_PRIZE_VEHICLE)
			{
				prize_successful_list [counter] = i;
				counter++;
			}
			else if(GetContainerData(container_id, EC_TYPE) == CONTAINER_TYPE_ACCESORYS)
			{
				if(GetPrizeData(i, EC_PRIZE_TYPE) == CONTAINER_TYPE_PRIZE_CLOTH
					|| GetPrizeData(i, EC_PRIZE_TYPE) == CONTAINER_TYPE_PRIZE_ACSESSORYS)
				{
					prize_successful_list [counter] = i;
					counter++;
				}
			}
			else if(GetContainerData(container_id, EC_TYPE) == CONTAINER_TYPE_GAME_VALUES)
			{
				if(GetPrizeData(i, EC_PRIZE_TYPE) == CONTAINER_TYPE_PRIZE_EXP
					|| GetPrizeData(i, EC_PRIZE_TYPE) == CONTAINER_TYPE_PRIZE_MONEY
					|| GetPrizeData(i, EC_PRIZE_TYPE) == CONTAINER_TYPE_PRIZE_DONATE)
				{
					prize_successful_list [counter] = i;
					counter++;
				}
			}
		}
	}

	if(counter > 0)
	{
		SetContainerData(container_id, EC_PRIZE, prize_successful_list [random(counter)]);
		if(counter > 1)
		{
			while(GetContainerData(container_id, EC_PRIZE) == GetContainerData(container_id, EC_LAST_PRIZE))
			{
				SetContainerData(container_id, EC_PRIZE, prize_successful_list [random(counter)]);
			}
		}

		SetContainerData(container_id, EC_LAST_PRIZE, GetContainerData(container_id, EC_PRIZE));
	}	

	SetContainerData(container_id, EC_START_PRICE, container_start_prices [GetContainerData(container_id, EC_CLASS)]);

	if(sql_update)
	{
		new temp_string[256];
		format
		(
			temp_string, sizeof(temp_string),
			"UPDATE `containers` SET `EC_CLASS` = '%d', `EC_TYPE` = '%d' \
			WHERE `EC_SQL_ID` = '%d' LIMIT 1",
			GetContainerData(container_id, EC_CLASS),
			GetContainerData(container_id, EC_TYPE),
			GetContainerData(container_id, EC_SQL_ID)
		);
		mysql_tquery(g_Handle, temp_string, "");
	}
}



// ---- Îáíîâëåíèå 3D-òåêñòà è äèíàìè÷åñêîé çîíû ïåðåä êîíòåéíåðîì
stock ContSystem:UpdateText(container_id)
{
	new temp_string[512];

	switch(GetContainerData(container_id, EC_STATUS))
	{
		case CONTAINER_STATUS_WAIT:
		{
			new current_hour, current_minute, temp_hour, temp_minute;
			
			gettime(current_hour, current_minute);
			sscanf(container_start_time [ContSystem:GetContStartTimeID(container_id)], "p<:>ii", temp_hour, temp_minute);

			new start_total_minutes = (current_hour*60 + current_minute) - (temp_hour*60 + temp_minute);
			start_total_minutes = abs(start_total_minutes);

			format
			(
				temp_string, sizeof(temp_string),
				""cl_grey"- Íà äàííûé ìîìåíò àóêöèîí íå ïðîõîäèò\n\
				"cl_orange"Áëèæàéøèé àóêöèîí áóäåò ÷åðåç "cl_white"%d ÷àñîâ(-à) %02d ìèíóò(-û)",
				start_total_minutes / 60,
				start_total_minutes % 60
			);
		}
		case CONTAINER_STATUS_AUCTION:
		{
			if(GetContainerData(container_id, EC_PLAYER_ID) == INVALID_PLAYER_ID)
			{
				format
				(
					temp_string, sizeof(temp_string),
					""cl_orange"Êîíòåéíåð ¹%d\n\
					"cl_grey"Êëàññ êîíòåéíåðà: "cl_white"%s\n\
					"cl_grey"Ñóììà íà÷àëüíîé ñòàâêè: "cl_white"%d$\n\n\
					"cl_grey"'Left ALT'",
					container_id+1,
					GetContainerClassStr(container_id),
					GetContainerData(container_id, EC_START_PRICE)
				);
			}
			else
			{
				new temp_name[MAX_PLAYER_NAME];
				GetPlayerName(GetContainerData(container_id, EC_PLAYER_ID), temp_name, MAX_PLAYER_NAME);

				format
				(
					temp_string, sizeof(temp_string),
					""cl_orange"Êîíòåéíåð ¹%d\n\
					"cl_grey"Êëàññ êîíòåéíåðà: "cl_white"%s\n\
					"cl_grey"Ñóììà íà÷àëüíîé ñòàâêè: "cl_white"%d$\n\
					"cl_grey"Òåêóùàÿ ñòàâêà: "cl_white"%d$\n\n\
					"cl_orange"Ïîñëåäíþþ ñòàâêó ïîñòàâèë: "cl_white"%s\n\
					"cl_orange"Äî êîíöà àóêöèîíà: "cl_white"%d ñåêóíä(-û)\n\n\
					"cl_grey"'Left ALT'",
					container_id+1,
					GetContainerClassStr(container_id),
					GetContainerData(container_id, EC_START_PRICE),
					GetContainerData(container_id, EC_PRICE),
					temp_name,
					GetContainerData(container_id, EC_STOP_TIMER)
				);
			}
		}
		case CONTAINER_STATUS_WINNER_WAIT:
		{
			new temp_name[MAX_PLAYER_NAME];
			GetPlayerName(GetContainerData(container_id, EC_PLAYER_ID), temp_name, MAX_PLAYER_NAME);

			format
			(
				temp_string, sizeof(temp_string),
				""cl_orange"Êîíòåéíåð îæèäàåò ñâîåãî ïîáåäèòåëÿ "cl_white"%s"cl_orange"!\n\n\
				"cl_grey"'Left ALT'",
				temp_name
			);
		}
	}

	new Float:temp_pos[2];
	temp_pos[0] = GetContainerData(container_id, EC_POS_X) + (container_text_offset[0] 
		* floatcos(GetContainerData(container_id, EC_POS_ROT), degrees) - container_text_offset[1] * floatsin(GetContainerData(container_id, EC_POS_ROT), degrees));
	temp_pos[1] = GetContainerData(container_id, EC_POS_Y) + (container_text_offset[0] 
		* floatsin(GetContainerData(container_id, EC_POS_ROT), degrees) + container_text_offset[1] * floatcos(GetContainerData(container_id, EC_POS_ROT), degrees));

	if(!IsValidDynamic3DTextLabel(GetContainerData(container_id, EC_TEXT_LABEL)))
	{
		GetContainerData(container_id, EC_TEXT_LABEL) = CreateDynamic3DTextLabel(temp_string, 
			-1, 
			temp_pos[0], 
			temp_pos[1], 
			GetContainerData(container_id, EC_POS_Z), 
			CONTAINER_TEXT_DRAW_DIST);

		GetContainerData(container_id, EC_DYNAMIC_AREA) = CreateDynamicSphere(temp_pos[0], 
			temp_pos[1], 
			GetContainerData(container_id, EC_POS_Z),
			CONTAINER_DYNAMIC_AREA_SIZE);
	}
	else 
	{
		UpdateDynamic3DTextLabelText(GetContainerData(container_id, EC_TEXT_LABEL), -1, temp_string);
	}
	return true;
}



// - Çàïóñê àóêöèîíà ó êîíòåéíåðà
stock ContSystem:StartAuction(container_id)
{
	if(GetContainerData(container_id, EC_STATUS) != CONTAINER_STATUS_WAIT)
	{
		return 0;
	}
	else
	{
		SetContainerData(container_id, EC_STATUS, CONTAINER_STATUS_AUCTION);
		SetContainerData(container_id, EC_STOP_AUCTION_TIME, gettime() + CONTAINER_ACUTION_TIME);

		ContSystem:CreateContObject(container_id);

		return ContSystem:UpdateText(container_id); 
	}
}



// - Óçíà¸ò ó êàêîãî êîíòåéíåðà íàõîäèòñÿ èãðîê
stock ContSystem:GetPlayerContID(playerid)
{
	new area[1];
	GetPlayerDynamicAreas(playerid, area);
	for(new i; i < MAX_CONTAINERS_COUNT; i++)
	{
		if(GetContainerData(i, EC_STATUS) == CONTAINER_STATUS_NONE
			|| area[0] != GetContainerData(i, EC_DYNAMIC_AREA)) 
		{
			continue;
		}
		
		return i;
	}
	return -1;
}



// - Ñîçäà¸ò íîâûé êîíòåéíåð äëÿ ðåäàêòèðîâàíèÿ åãî ïîçèöèè
stock ContSystem:EditPlayerNewCont(playerid)
{
	new Float:temp_pos[4];
	GetPlayerPos(playerid, temp_pos[0], temp_pos[1], temp_pos[2]);
	GetPlayerFacingAngle(playerid, temp_pos[3]);

	new Float:a; 
    temp_pos[0] += (6.0 * floatsin(-temp_pos[3], degrees)); 
    temp_pos[1] += (6.0 * floatcos(-temp_pos[3], degrees)); 
	
	edit_player_container_id [playerid] = CreateObject(CONTAINER_OBJECT_ID, 
		temp_pos[0], 
		temp_pos[1], 
		temp_pos[2], 
		0.0, 
		0.0, 
		0.0);
	return EditObject(playerid, edit_player_container_id [playerid]);
}

// - Âûäà¸ò èãðîêó ïîáåäèâøåìó â àóêöèîíå ïðèç
stock ContSystem:GiveContainerPrize(container_id)
{
	switch(GetContainerData(container_id, EC_TYPE))
	{
		case CONTAINER_TYPE_CAR:
		{
			SendClientMessageEx(GetContainerData(container_id, EC_PLAYER_ID), -1, " "cl_orange"Âû âûèãðàëè òðàíñïîðò "cl_white"'%s'", GetVehicleName(GetPrizeData(GetContainerData(container_id, EC_PRIZE), EC_PRIZE_VALUE)-400));
			
			/*
				Ëîãèðîâàíèå: èãðîê %d çàáðàë âûèãðàííûé òðàíñïîðò %d 

				GetContainerData(container_id, EC_PLAYER_ID)
				GetPrizeData(GetContainerData(container_id, EC_PRIZE), EC_PRIZE_VALUE)
			*/



			/*

				ÂÛÄÀÒÜ ÈÃÐÎÊÓ ÒÐÀÍÑÏÎÐÒ (ID - accesories_priz [GetContainerData(container_id, EC_CLASS)][GetContainerData(container_id, EC_PRIZE)][0])

			*/
		}
		case CONTAINER_TYPE_ACCESORYS:
		{
			SendClientMessageEx(GetContainerData(container_id, EC_PLAYER_ID), -1, " "cl_orange"Âû âûèãðàëè %s ID %d", (GetPrizeData(GetContainerData(container_id, EC_PRIZE), EC_PRIZE_TYPE) == CONTAINER_TYPE_PRIZE_CLOTH) ? ("îäåæäó") : ("àêñåññóàð"), GetPrizeData(GetContainerData(container_id, EC_PRIZE), EC_PRIZE_VALUE));
		
			/*
				Âûäàòü èãðîêó %d ñêèí/àêñåññóð
				
				GetContainerData(container_id, EC_PLAYER_ID)

				if(GetPrizeData(GetContainerData(container_id, EC_PRIZE), EC_PRIZE_TYPE) == CONTAINER_TYPE_PRIZE_CLOTH)
				{
					Âûäà¸ì ÑÊÈÍ %d
					GetPrizeData(GetContainerData(containerid, EC_PRIZE), EC_PRIZE_VALUE)
				}
				else
				{
					Âûäà¸ì ÀÊÑÅÑÑÓÀÐ %d
					GetPrizeData(GetContainerData(containerid, EC_PRIZE), EC_PRIZE_VALUE)
				}

			*/



			/*
				Ëîãèðîâàíèå: èãðîê %d çàáðàë âûèãðàííûé àêñåññóàð èëè ñêèí %d 

				GetContainerData(container_id, EC_PLAYER_ID)
				GetPrizeData(GetContainerData(container_id, EC_PRIZE), EC_PRIZE_VALUE)
			*/
		}
		case CONTAINER_TYPE_GAME_VALUES:
		{
			switch(GetPrizeData(GetContainerData(container_id, EC_PRIZE), EC_PRIZE_TYPE))
			{
				case CONTAINER_TYPE_PRIZE_EXP:
				{
					/*
						Âûäàòü èãðîêó %d îïûò â êîëè÷åñòâå %d

						GetContainerData(container_id, EC_PLAYER_ID)
						GetPrizeDate(GetContainerData(conatiner_id, EC_PRIZE), EC_PRIZE_VALUE)

					*/



					/*
						Ëîãèðîâàíèå: èãðîê %d âûèãðàë îïûò â ðàçìåðå %d

						GetContainerData(container_id, EC_PLAYER_ID)
						GetPrizeDate(GetContainerData(conatiner_id, EC_PRIZE), EC_PRIZE_VALUE)
					*/
				}
				case CONTAINER_TYPE_PRIZE_MONEY:
				{
					GiveMoney(GetContainerData(container_id, EC_PLAYER_ID), GetPrizeData(GetContainerData(container_id, EC_PRIZE), EC_PRIZE_VALUE));

					/*
						Ëîãèðîâàíèå: èãðîê %d âûèãðàë äåíåæíûå ñðåäñòâà â ðàçìåðå %d$

						GetContainerData(container_id, EC_PLAYER_ID)
						GetPrizeDate(GetContainerData(conatiner_id, EC_PRIZE), EC_PRIZE_VALUE)
					*/
				}
				case CONTAINER_TYPE_PRIZE_DONATE:
				{
					/*
						Âûäàèòü èãðîêó %d äîíàò â êîëè÷åñòâå %d

						GetContainerData(container_id, EC_PLAYER_ID)
						GetPrizeDate(GetContainerData(conatiner_id, EC_PRIZE), EC_PRIZE_VALUE)

					*/
					/*
						Ëîãèðîâàíèå: èãðîê %d âûèãðàë äîíàò â ðàçìåðå %d

						GetContainerData(container_id, EC_PLAYER_ID)
						GetPrizeDate(GetContainerData(conatiner_id, EC_PRIZE), EC_PRIZE_VALUE)
					*/
				}
			}
			

			SendClientMessageEx(GetContainerData(container_id, EC_PLAYER_ID), -1, " "cl_orange"Âû âûèãðàëè %s (%d)", container_prize_types_str [GetPrizeData(GetContainerData(container_id, EC_PRIZE), EC_PRIZE_TYPE)], GetPrizeData(GetContainerData(container_id, EC_PRIZE), EC_PRIZE_VALUE));
		}
	}

	ContSystem:DeleteContObject(container_id);

	SetContainerData(container_id, EC_PLAYER_ID, INVALID_PLAYER_ID);
	SetContainerData(container_id, EC_STATUS, CONTAINER_STATUS_WAIT);
	ContSystem:UpdatePrize(container_id, true, true);
	ContSystem:UpdateText(container_id);
}



// ---- Ïðîäà¸ò âûèãðàííûé ïðèç
stock ContSystem:SellContainerPrize(container_id)
{
	new sell_prize_price = floatround(CONTAINER_PRIZE_SELL_PERCENT*GetPrizeData(GetContainerData(container_id, EC_PRIZE), EC_PRIZE_PRICE));

	SendClientMessageEx(GetContainerData(container_id, EC_PLAYER_ID), -1, " "cl_orange"Âû ïðîäàëè ïðèç çà "cl_white"%d$", sell_prize_price);
	GiveMoney(GetContainerData(container_id, EC_PLAYER_ID), sell_prize_price);

	ContSystem:DeleteContObject(container_id);

	SetContainerData(container_id, EC_PLAYER_ID, INVALID_PLAYER_ID);
	SetContainerData(container_id, EC_STATUS, CONTAINER_STATUS_WAIT);
	ContSystem:UpdatePrize(container_id, true, true);
	ContSystem:UpdateText(container_id);
}



// ---- Îòêðûâàåò êîíòåéíåð
stock ContSystem:OpenContainer(playerid, container_id)
{
	new Float:temp_rot[2][3];
	GetObjectRot(GetContainerData(container_id, EC_DOOR_OBJECT_1), 
		temp_rot[0][0], temp_rot[0][1], temp_rot[0][2]);
	SetObjectRot(GetContainerData(container_id, EC_DOOR_OBJECT_1), 
		temp_rot[0][0], temp_rot[0][1], temp_rot[0][2]+150.0);

	GetObjectRot(GetContainerData(container_id, EC_DOOR_OBJECT_2), 
		temp_rot[1][0], temp_rot[1][1], temp_rot[1][2]);
	SetObjectRot(GetContainerData(container_id, EC_DOOR_OBJECT_2), 
		temp_rot[1][0], temp_rot[1][1], temp_rot[1][2]-150.0);

	switch(GetContainerData(container_id, EC_TYPE))
	{
		case CONTAINER_TYPE_CAR:
		{
			GetContainerData(container_id, EC_PRIZE_OBJECT) = CreateVehicle(
				GetPrizeData(GetContainerData(container_id, EC_PRIZE), EC_PRIZE_VALUE),
				GetContainerData(container_id, EC_POS_X),
				GetContainerData(container_id, EC_POS_Y),
				GetContainerData(container_id, EC_POS_Z),
				GetContainerData(container_id, EC_POS_ROT)+180.0,
				random(255), 
				random(255),
				-1
			);
		}
		case CONTAINER_TYPE_ACCESORYS:
		{
			GetContainerData(container_id, EC_PRIZE_OBJECT) = CreateObject(
				(GetPrizeData(GetContainerData(container_id, EC_PRIZE), EC_PRIZE_TYPE) == CONTAINER_TYPE_PRIZE_CLOTH) ? (CONTAINER_CLOTH_OBJECT) : (CONTAINER_ACCESORY_OBJECT),
				GetContainerData(container_id, EC_POS_X),
				GetContainerData(container_id, EC_POS_Y),
				(GetPrizeData(GetContainerData(container_id, EC_PRIZE), EC_PRIZE_TYPE) == CONTAINER_TYPE_PRIZE_CLOTH) ? (GetContainerData(container_id, EC_POS_Z)-1.0) : (GetContainerData(container_id, EC_POS_Z)-1.25),
				0.0,
				0.0,
				GetContainerData(container_id, EC_POS_ROT)+180.0
			);
		}
		case CONTAINER_TYPE_GAME_VALUES:
		{
			GetContainerData(container_id, EC_PRIZE_OBJECT) = CreateObject(
				CONTAINER_GAME_VALUES_OBJ,
				GetContainerData(container_id, EC_POS_X),
				GetContainerData(container_id, EC_POS_Y),
				GetContainerData(container_id, EC_POS_Z)-1.0,
				0.0,
				0.0,
				GetContainerData(container_id, EC_POS_ROT)+180.0
			);
		}
	}

	SetContainerData(container_id, EC_STATUS, CONTAINER_STATUS_TAKE_PRIZE);
	SetPlayerDataEx(playerid, P_CONT_TAKE_PRIZE_TIME, CONTAINER_TAKE_PRIZE_TIME);
	SetPlayerDataEx(playerid, P_TAKE_PRIZE_CONTAINER_ID, container_id);
	return true;
}




// ---- Ñîçäàíèå îþúåêòà êîíòåéíåðà
stock ContSystem:CreateContObject(container_id)
{
	GetContainerData(container_id, EC_OBJECT_ID) = CreateObject(CONTAINER_OBJECT_ID, 
		GetContainerData(container_id, EC_POS_X), 
		GetContainerData(container_id, EC_POS_Y), 
		GetContainerData(container_id, EC_POS_Z), 
		0.0,
		0.0, 
		GetContainerData(container_id, EC_POS_ROT));

	new Float:temp_x[2], Float:temp_y[2];
	temp_x[0] = GetContainerData(container_id, EC_POS_X) 
		+ (container_doors_offset[0][0] 
		* floatcos(GetContainerData(container_id, EC_POS_ROT), degrees) 
		- container_doors_offset[0][1] 
		* floatsin(GetContainerData(container_id, EC_POS_ROT), degrees));
	temp_y[0] = GetContainerData(container_id, EC_POS_Y) 
		+ (container_doors_offset[0][0] 
		* floatsin(GetContainerData(container_id, EC_POS_ROT), degrees) 
		+ container_doors_offset[0][1] 
		* floatcos(GetContainerData(container_id, EC_POS_ROT), degrees));

	
	temp_x[1] = GetContainerData(container_id, EC_POS_X) + (container_doors_offset[1][0] 
		* floatcos(GetContainerData(container_id, EC_POS_ROT), degrees) 
		- container_doors_offset[1][1] 
		* floatsin(GetContainerData(container_id, EC_POS_ROT), degrees));
	temp_y[1] = GetContainerData(container_id, EC_POS_Y) 
		+ (container_doors_offset[1][0] 
		* floatsin(GetContainerData(container_id, EC_POS_ROT), degrees) 
		+ container_doors_offset[1][1] 
		* floatcos(GetContainerData(container_id, EC_POS_ROT), degrees));

	GetContainerData(container_id, EC_DOOR_OBJECT_1) = CreateObject(CONTAINER_DOOR_OBJECT_ID, 
		temp_x[0], 
		temp_y[0], 
		GetContainerData(container_id, EC_POS_Z) + container_doors_offset[0][2], 
		0.0,
		0.0, 
		GetContainerData(container_id, EC_POS_ROT) + container_doors_offset[0][3]);

	GetContainerData(container_id, EC_DOOR_OBJECT_2) = CreateObject(CONTAINER_DOOR_OBJECT_ID, 
		temp_x[1], 
		temp_y[1],  
		GetContainerData(container_id, EC_POS_Z) + container_doors_offset[1][2], 
		0.0,
		0.0, 
		GetContainerData(container_id, EC_POS_ROT)+container_doors_offset[1][3]);

	SetObjectMaterial(GetContainerData(container_id, EC_DOOR_OBJECT_1), 0, -1, "none", "none", 0xFFFFFFFF);
	SetObjectMaterial(GetContainerData(container_id, EC_DOOR_OBJECT_2), 0, -1, "none", "none", 0xFFFFFFFF);
}




// ---- Óäàëÿåò îáúåêò êîíòåéíåðà
stock ContSystem:DeleteContObject(container_id)
{
	DestroyObject(GetContainerData(container_id, EC_OBJECT_ID));
	GetContainerData(container_id, EC_OBJECT_ID) = INVALID_OBJECT_ID;

	DestroyObject(GetContainerData(container_id, EC_DOOR_OBJECT_1));
	GetContainerData(container_id, EC_DOOR_OBJECT_1) = INVALID_OBJECT_ID;

	DestroyObject(GetContainerData(container_id, EC_DOOR_OBJECT_2));
	GetContainerData(container_id, EC_DOOR_OBJECT_2) = INVALID_OBJECT_ID;

	if(IsValidVehicle(GetContainerData(container_id, EC_PRIZE_OBJECT)))
	{
		DestroyVehicle(GetContainerData(container_id, EC_PRIZE_OBJECT));
	}
	else if(IsValidObject(GetContainerData(container_id, EC_PRIZE_OBJECT)))
	{
		DestroyObject(GetContainerData(container_id, EC_PRIZE_OBJECT));
	}
	SetContainerData(container_id, EC_PRIZE_OBJECT, INVALID_OBJECT_ID);
}



// ---- Îáíîâëÿåò êîîðäèíàòû êîíòåéíåðà â áàçå äàííûõ
stock ContSystem:UpdateContainerPos(container_id)
{
	new temp_string[256];
	format
	(
		temp_string, sizeof(temp_string),
		"UPDATE `containers` SET `EC_POS_X` = '%f', `EC_POS_Y` = '%f', `EC_POS_Z` = '%f', `EC_POS_ROT` = '%f' \
		WHERE `EC_SQL_ID` = '%d' LIMIT 1",
		GetContainerData(container_id, EC_POS_X),
		GetContainerData(container_id, EC_POS_Y),
		GetContainerData(container_id, EC_POS_Z),
		GetContainerData(container_id, EC_POS_ROT),
		GetContainerData(container_id, EC_SQL_ID)
	);
	mysql_tquery(g_Handle, temp_string, "");
}



// ---- Äîáàâëÿåò è çàãðóæàåò íîâûé êîíòåéíåð â (èç) áàçû äàííûõ
stock ContSystem:AddNewContainer(Float:fX, Float:fY, Float:fZ, Float:fRotZ)
{
	new temp_string[256];
	format
	(
		temp_string, sizeof(temp_string),
		"INSERT INTO `containers` (`EC_POS_X`, `EC_POS_Y`, `EC_POS_Z`, `EC_POS_ROT`) \
		VALUES ('%f', '%f', '%f', '%f')",
		fX, fY, fZ, fRotZ
	);
	mysql_tquery(g_Handle, temp_string, "");
	mysql_tquery(g_Handle, "SELECT * FROM `containers` ORDER BY `EC_SQL_ID` DESC LIMIT 1", "LoadNewContainer");
}



// ---- Ïîëíîå óäàëåíèå êîíòåéíåðà
stock ContSystem:DeleteContainer(container_id)
{
	SetContainerData(container_id, EC_STATUS, CONTAINER_STATUS_NONE);
	ContSystem:DeleteContObject(container_id);
	DestroyDynamicArea(GetContainerData(container_id, EC_DYNAMIC_AREA));
	DestroyDynamic3DTextLabel(GetContainerData(container_id, EC_TEXT_LABEL));

	new temp_string[128];
	format
	(
		temp_string, sizeof(temp_string),
		"DELETE FROM `containers` WHERE `EC_SQL_ID` = '%d' LIMIT 1",
		GetContainerData(container_id, EC_SQL_ID)
	);
	mysql_tquery(g_Handle, temp_string, "");

	g_container_data [container_id] = g_container_data_default;
}



// ---- Äîáàâëåíèå íîâîãî ïðèçà â ñòðóêòóðó
stock ContSystem:AddNewPrize(prize_sql_id, prize_class, prize_type, prize_value, prize_price = 0)
{
	for(new prize_id; prize_id < MAX_CONTAINER_PRIZES; prize_id++)
	{
		if(GetPrizeData(prize_id, EC_PRIZE_SQL_ID) != -1) continue;

		SetPrizeData(prize_id, EC_PRIZE_SQL_ID, prize_sql_id);
		SetPrizeData(prize_id, EC_PRIZE_CLASS, prize_class);
		SetPrizeData(prize_id, EC_PRIZE_TYPE, prize_type);
		SetPrizeData(prize_id, EC_PRIZE_VALUE, prize_value);
		SetPrizeData(prize_id, EC_PRIZE_PRICE, prize_price);
		break;
	}
}



// ---- Äîáàâëåíèå è çàãðóçêà íîâîãî ïðèçà â (èç) ÁÄ
stock ContSystem:CreateNewPrize(prize_class, prize_type, prize_value, prize_price = 0)
{
	new temp_string[256];

	format
	(
		temp_string, sizeof(temp_string),
		"INSERT INTO `container_prizes` (`EC_PRIZE_CLASS`, `EC_PRIZE_TYPE`, `EC_PRIZE_VALUE`, `EC_PRIZE_PRICE`) \
		VALUES ('%d', '%d', '%d', '%d')",
		prize_class, prize_type, prize_value, prize_price
	);
	mysql_tquery(g_Handle, temp_string, "");
	mysql_tquery(g_Handle, "SELECT * FROM `container_prizes` ORDER BY `EC_PRIZE_SQL_ID` DESC LIMIT 1", "LoadNewPrize");
}



// ---- Îáíîâëåíèå ïðèçà â áàçå äàííûõ
stock ContSystem:UpdatePrizeSQL(prize_id)
{
	new temp_string[512];
	format
	(
		temp_string, sizeof(temp_string),
		"UPDATE `container_prizes` SET `EC_PRIZE_CLASS` = '%d', `EC_PRIZE_TYPE` = '%d', `EC_PRIZE_VALUE` = '%d', \
		`EC_PRIZE_PRICE` = '%d' WHERE `EC_PRIZE_SQL_ID` = '%d' LIMIT 1",
		GetPrizeData(prize_id, EC_PRIZE_CLASS),
		GetPrizeData(prize_id, EC_PRIZE_TYPE),
		GetPrizeData(prize_id, EC_PRIZE_VALUE),
		GetPrizeData(prize_id, EC_PRIZE_PRICE),
		GetPrizeData(prize_id, EC_PRIZE_SQL_ID)
	);
	mysql_tquery(g_Handle, temp_string, "");
}



// ---- Ïîëíîå óäàëåíèå ïðèçà
stock ContSystem:DeletePrize(prize_id)
{
	new temp_string[256];
	format
	(
		temp_string, sizeof(temp_string),
		"DELETE FROM `container_prizes` WHERE `EC_PRIZE_SQL_ID` = '%d' LIMIT 1",
		GetPrizeData(prize_id, EC_PRIZE_SQL_ID)
	);
	mysql_tquery(g_Handle, temp_string, "", "");

	g_prizes_data [prize_id] = g_prizes_data_default;
}

//==============================================================================

// ---- Îòêðûâàåò ãëàâíîå ìåíþ êîíòåéíåðîâ
stock ContSystem:ShowContMenu(playerid)
{
	return ShowPlayerDialog(playerid, DIALOG_CONTMENU, DIALOG_STYLE_LIST,
		""cl_orange"Ìåíþ êîíòåéíåðîâ",
		""cl_orange"- "cl_white"Ñïèñîê êîíòåéíåðîâ\n\
		"cl_orange"- "cl_white"Ñîçäàíèå êîíòåéíåðà\n\
		"cl_orange"- "cl_white"Ðåäàêòèðîâàíèÿ ñïèñêà ïðèçîâ",
		"Âûáðàòü", "Çàêðûòü");
}



// ---- Îòêðûâàåò ñïèñîê ñîóùåñòâóþùèõ êîíòåéíåðîâ
stock ContSystem:ShowContListMenu(playerid)
{
	STRING_GLOBAL[0] = EOS;
	new counter;

	format
	(
		STRING_GLOBAL, sizeof(STRING_GLOBAL),
		"Êîíòåéíåð\tÑòàòóñ"
	);

	for(new i; i < MAX_CONTAINERS_COUNT; i++)
	{
		if(GetContainerData(i, EC_STATUS) != CONTAINER_STATUS_NONE)
		{
			format
			(
				STRING_GLOBAL, sizeof(STRING_GLOBAL),
				"%s\n"cl_white"Êîíòåéíåð "cl_orange"¹%d\t"cl_grey"[%s]",
				STRING_GLOBAL, i+1, GetContainerStatusStr(i)
			);
			edit_player_container_listitem [playerid][counter] = i;
			counter++;
		}
	}

	if(counter <= 0)
		return SendErrorMessage(playerid, "Íà äàííûé ìîìåíò íà ñåðâåðå íåò êîíòåéíåðîâ");
	else return ShowPlayerDialog(playerid, DIALOG_CONTMENU_LIST, DIALOG_STYLE_TABLIST_HEADERS,
		""cl_orange"Ìåíþ óïðàâëåíèÿ êîíòåéíåðàìè", STRING_GLOBAL,
		"Äàëåå", "Çàêðûòü");
}



// ---- Îòêðûâàåò ìåíþ ðåäàêòèðîâàíèÿ êîíòåéíåðà
stock ContSystem:ShowContEdit(playerid)
{
	STRING_GLOBAL[0] = EOS;

	format
	(
		STRING_GLOBAL, sizeof(STRING_GLOBAL),
		""cl_orange"- "cl_white"Íà÷àòü àóêöèîí\n\
		"cl_orange"- "cl_white"Çàâåðøèòü àóêöèîí\n\
		"cl_orange"- "cl_white"Ðåäàêòèðîâàíèå ïðèçà\n\
		"cl_orange"- "cl_white"Èçìåíèòü ìåñòîïîëîæåíèå êîíòåéíåðà\n\
		"cl_orange"- "cl_white"Óäàëèòü êîíòåéíåð"
	);
	return ShowPlayerDialog(playerid, DIALOG_CONTMENU_EDIT, DIALOG_STYLE_LIST,
		""cl_orange"Ðåäàêòèðîâàíèå êîíòåéíåðà", STRING_GLOBAL,
		"Âûáðàòü", "Íàçàä");
}



// ---- Îòêðûâàåò äèàëîã ïîäòâåðæäåíèÿ ñòàâêè íà êîíòåéíåð
stock ContSystem:ShowAcceptBid(playerid, container_id)
{
	STRING_GLOBAL[0] = EOS;

	if(GetContainerData(container_id, EC_PLAYER_ID) != INVALID_PLAYER_ID)
	{
		format
		(
			STRING_GLOBAL, sizeof(STRING_GLOBAL),
			""cl_orange"- "cl_white"Òåêóùàÿ ñóììà ñòàâêè "cl_grey"%d$\n\n\
			"cl_grey"Âû æåëàåòå ïîñòàâèòü "cl_white"%d$ (+%d) "cl_grey"íà êîíòåéíåð "cl_white"¹%d"cl_grey"?",
			GetContainerData(container_id, EC_PRICE),
			GetContainerData(container_id, EC_PRICE) + CONTAINER_BID_PRICE,
			CONTAINER_BID_PRICE,
			container_id+1
		);
	}
	else
	{
		format
		(
			STRING_GLOBAL, sizeof(STRING_GLOBAL),
			""cl_orange"- "cl_white"Ìèíèìàëüíàÿ ñóììà ñòàâêè äëÿ äàííîãî êîíòåéíåðà ñîñòàâëÿåò "cl_grey"%d$\n\n\
			"cl_grey"Âû äåéñòâèòåëüíî õîòèòå ïîñòàâèòü "cl_white"%d$ íà êîíòåéíåð "cl_grey"¹%d?",
			GetContainerData(container_id, EC_START_PRICE),
			GetContainerData(container_id, EC_START_PRICE),
			container_id+1
		);
	}
	return ShowPlayerDialog(playerid, DIALOG_CONT_ACCEPT_BID, DIALOG_STYLE_MSGBOX,
		""cl_orange"Àóêöèîí êîíòåéíåðîâ", STRING_GLOBAL,
		"Äà", "Íåò");
}



// ---- Îòêðûâàåò äèàëîã äåéñòâèÿ ñ ïðèçîì äëÿ ïîáåäèâøåãî èãðîêà
stock ContSystem:ShowTakePrizeDialog(playerid)
{
	new temp_string[512];
	if(GetPlayerDataEx(playerid, P_TAKE_PRIZE_CONTAINER_ID) == -1)
	{
		return false;
	}

	switch(GetContainerData(GetPlayerDataEx(playerid, P_TAKE_PRIZE_CONTAINER_ID), EC_TYPE))
	{
		case CONTAINER_TYPE_CAR:
		{
			format
			(
				temp_string, sizeof(temp_string),
				""cl_white"Âû æåëàåòå çàáðàòü òàíñïîðò "cl_orange"'%s'"cl_white"èëè ïðîäàòü â ôîíä ïîðòà çà "cl_orange"%d$"cl_white"?",
				GetVehicleName(GetPrizeData(GetContainerData(GetPlayerDataEx(playerid, P_TAKE_PRIZE_CONTAINER_ID), EC_PRIZE), EC_PRIZE_VALUE)-400),
				floatround(CONTAINER_PRIZE_SELL_PERCENT*GetPrizeData(GetContainerData(GetPlayerDataEx(playerid, P_TAKE_PRIZE_CONTAINER_ID), EC_PRIZE), EC_PRIZE_PRICE))
			);
		}
		case CONTAINER_TYPE_ACCESORYS:
		{
			format
			(
				temp_string, sizeof(temp_string),
				""cl_white"Âû æåëàåòå çàáðàòü "cl_orange"%s ID %d "cl_white"èëè ïðîäàòü â ôîíä ïîðòà çà "cl_orange"%d$"cl_white"?",
				(GetPrizeData(GetContainerData(GetPlayerDataEx(playerid, P_TAKE_PRIZE_CONTAINER_ID), EC_PRIZE), EC_PRIZE_TYPE) == CONTAINER_TYPE_PRIZE_CLOTH) ? ("îäåæäó") : ("àêñåññóàð"),
				GetPrizeData(GetContainerData(GetPlayerDataEx(playerid, P_TAKE_PRIZE_CONTAINER_ID), EC_PRIZE), EC_PRIZE_VALUE),
				floatround(CONTAINER_PRIZE_SELL_PERCENT*GetPrizeData(GetContainerData(GetPlayerDataEx(playerid, P_TAKE_PRIZE_CONTAINER_ID), EC_PRIZE), EC_PRIZE_PRICE))
			);
		}
		case CONTAINER_TYPE_GAME_VALUES:
		{
			ContSystem:GiveContainerPrize(GetPlayerDataEx(playerid, P_TAKE_PRIZE_CONTAINER_ID));
			SetPlayerDataEx(playerid, P_TAKE_PRIZE_CONTAINER_ID, -1);
		}
	}
	return ShowPlayerDialog(playerid, DIALOG_CONTAINER_TAKE_PRIZE, DIALOG_STYLE_MSGBOX,
		""cl_orange"Âûáåðèòå äåéñòâèå ñ ïðèçîì", temp_string,
		"Çàáðàòü", "Ïðîäàòü");
}



// ---- Îòêðûâàåò ìåíþ ðåäàêòèðîâàíèÿ ïðèçà â êîíêðåòíîì êîíòåéíåðå
stock ContSystem:ShowEditPrizeMenu(playerid, container_id)
{
	new temp_string[512];
	
	format
	(
		temp_string, sizeof(temp_string),
		"Ïóíêò\tÇíà÷åíèå\n\
		"cl_white"Êëàññ êîíòåéíåðà\t"cl_orange"["cl_grey"%s"cl_orange"]\n\
		"cl_white"Òèï êîíòåéíåðà\t"cl_orange"["cl_grey"%s"cl_orange"]",
		GetContainerClassStr(container_id),
		GetContainerTypeStr(container_id)

	);

	ContSystem:UpdateText(container_id);
	return ShowPlayerDialog(playerid, DIALOG_CONTMENU_EDIT_PRIZE, DIALOG_STYLE_TABLIST_HEADERS,
		""cl_orange"Ðåäàêòèðîâàíèå ïðèçà", temp_string,
		"Âûáðàòü", "Çàêðûòü");
}



// ---- Îòêðûâàåò ìåíþ èçìåíåíèÿ êëàññà êîíêðåòíîãî êîíòåéíåðà
stock ContSystem:ShowChangeContClass(playerid, container_id)
{
	new temp_string[128];
	for(new i = 1; i < CONTAINER_CLASSES_COUNT+1; i++)
	{
		format
		(
			temp_string, sizeof(temp_string),
			"%s\n"cl_orange"%d. "cl_white"%s",
			temp_string,
			i,
			container_class_str[i]
		);
	}
	return ShowPlayerDialog(playerid, DIALOG_CNTM_EDIT_PRIZE_CLASS, DIALOG_STYLE_LIST,
		""cl_orange"Âûáåðèòå êëàññ êîíòåéíåðà", temp_string,
		"Âûáðàòü", "Íàçàä");
}



// ---- Îòêðûâàåò ìåíþ èçìåíåíèÿ òèïà êîíêðåòíîãî êîíòåéíåðà
stock ContSystem:ShowChangeContType(playerid, container_id)
{
	new temp_string[128];
	for(new i = 1; i < CONTAINER_PRIZE_TYPE_COUNT+1; i++)
	{
		format
		(
			temp_string, sizeof(temp_string),
			"%s\n"cl_orange"%d. "cl_white"%s",
			temp_string,
			i,
			container_type_str[i]
		);
	}
	return ShowPlayerDialog(playerid, DIALOG_CNTM_EDIT_PRIZE_TYPE, DIALOG_STYLE_LIST,
		""cl_orange"Âûáåðèòå òèï êîíòåéíåðà", temp_string,
		"Âûáðàòü", "Íàçàä");
}



// ---- Îòêðûâàåò ñïèñîê âñåõ ïðèçîâ (ñ âîçìîæíîñòü äîáàâëåíèÿ íîâîãî ïðèçà)
stock ContSystem:ShowPrizeList(playerid)
{
	STRING_GLOBAL[0] = EOS;
	new counter;

	format
	(
		STRING_GLOBAL, sizeof(STRING_GLOBAL),
		""cl_orange"- Äîáàâèòü ïðèç"
	);

	for(new prize_id; prize_id < MAX_CONTAINER_PRIZES; prize_id++)
	{
		if(GetPrizeData(prize_id, EC_PRIZE_SQL_ID) == -1) continue;

		format
		(
			STRING_GLOBAL, sizeof(STRING_GLOBAL),
			"%s\n"cl_white"%s "cl_grey"(%d) "cl_white"- %s êëàññ",
			STRING_GLOBAL,
			container_prize_types_str [GetPrizeData(prize_id, EC_PRIZE_TYPE)],
			GetPrizeData(prize_id, EC_PRIZE_VALUE),
			container_class_str [GetPrizeData(prize_id, EC_PRIZE_CLASS)]
		);

		edit_player_container_listitem [playerid][counter] = prize_id;
		counter++;
	}
	return ShowPlayerDialog(playerid, DIALOG_CONTAINER_PRIZE_LIST, DIALOG_STYLE_LIST,
		""cl_orange"Ñïèñîê ïðèçîâ", STRING_GLOBAL,
		"Äàëåå", "Çàêðûòü");
}



// ---- Îòêðûâàåò âûáîð êëàññà äëÿ íîâîãî ïðèçà
stock ContSystem:ShowAddNewPrize(playerid)
{
	new temp_string[256];
	for(new i = 1; i < CONTAINER_CLASSES_COUNT+1; i++)
	{
		format
		(
			temp_string, sizeof(temp_string),
			"%s\n%s",
			temp_string,
			container_class_str [i]
		);
	}
	return ShowPlayerDialog(playerid, DIALOG_CONT_PRIZE_ADD_CLASS, DIALOG_STYLE_LIST,
		""cl_orange"Âûáåðèòå êëàññ ïðèçà", temp_string,
		"Äàëåå", "Íàçàä");
}



// ---- Îòêðûâàåò âûáîð òèïà äëÿ íîâîãî ïðèçà
stock ContSystem:ShowAddNewPrizeType(playerid)
{
	new temp_string[512];
	for(new i = 1; i < sizeof(container_prize_types_str); i++)
	{
		format
		(
			temp_string, sizeof(temp_string),
			"%s\n%s",
			temp_string,
			container_prize_types_str [i]
		);
	}
	return ShowPlayerDialog(playerid, DIALOG_CONT_PRIZE_ADD_TYPE, DIALOG_STYLE_LIST,
		""cl_orange"Âûáåðèòå òèï ïðèçà", temp_string,
		"Äàëåå", "Íàçàä");
}



// ---- Îòêðûâàåò ïîëå ââîäà íîâîãî ID (çíà÷åíèÿ) ïðèçà
stock ContSystem:ShowAddNewPrizeValue(playerid, error_string[] = "")
{
	new temp_string[512];
	format
	(
		temp_string, sizeof(temp_string),
		""cl_orange"- "cl_white"Ââåäèòå ID òðàíñïîðòà, îäåæäû, àêñåññóàðà èëè êîëè÷åñòâî èãðîâîé öåííîñòè\n\n\
		%s",
		error_string
	);
	return ShowPlayerDialog(playerid, DIALOG_CONT_PRIZE_ADD_VALUE, DIALOG_STYLE_INPUT,
		""cl_orange"Äîáàâëåíèå íîâîãî ïðèçà", temp_string,
		"Äàëåå", "Íàçàä");
}



// ---- Îòêðûâàåò ïîëå ââîäà ãîñ. ñòîèìîñòè íîâîãî ïðèçà
stock ContSystem:ShowAddNewPrizePrice(playerid, error_string[] = "")
{
	new temp_string[512];
	format
	(
		temp_string, sizeof(temp_string),
		""cl_orange"- "cl_white"Ââåäèòå ïîëíóþ ãîñ. ñòîèìîñòü äàííîãî ïðèçà\n\n\
		%s",
		error_string
	);
	return ShowPlayerDialog(playerid, DIALOG_CONT_PRIZE_ADD_PRICE, DIALOG_STYLE_INPUT,
		""cl_orange"Äîáàâëåíèå íîâîãî ïðèçà", temp_string,
		"Äàëåå", "Íàçàä");
}



// ---- Îòêðûâàåò ìåíþ ðåäàêòèðîâàíèÿ êîíêðåòíîãî ïðèçà
stock ContSystem:ShowEditPrize(playerid, prize_id)
{
	new temp_string[512];
	format
	(
		temp_string, sizeof(temp_string),
		"Ïàðàìåòð\tÇíà÷åíèå\n\
		"cl_white"Êëàññ ïðèçà\t"cl_orange"["cl_grey"%s"cl_orange"]\n\
		"cl_white"Òèï ïðèçà\t"cl_orange"["cl_grey"%s"cl_orange"]\n\
		"cl_white"Âûäàâàåìûé ID/êîëè÷åñòâî\t"cl_orange"["cl_grey"%d"cl_orange"]\n\
		"cl_white"Ïîëíàÿ ãîñ. ñòîèìîñòü\t"cl_orange"["cl_grey"%d$"cl_orange"]\n\
		"cl_orange"- Óäàëèòü ïðèç",
		container_class_str [GetPrizeData(prize_id, EC_PRIZE_CLASS)],
		container_prize_types_str [GetPrizeData(prize_id, EC_PRIZE_TYPE)],
		GetPrizeData(prize_id, EC_PRIZE_VALUE),
		GetPrizeData(prize_id, EC_PRIZE_PRICE)
	);
	return ShowPlayerDialog(playerid, DIALOG_CONTMENU_PRIZE_EDIT, DIALOG_STYLE_TABLIST_HEADERS,
		""cl_orange"Ðåäàêòèðîâàíèå ïðèçà", temp_string,
		"Âûáðàòü", "Íàçàä");
}



// ---- Îòêðûâàåò ìåíþ ðåäàêòèðîâàíèÿ êëàññà êîíêðåòíîãî ïðèçà
stock ContSystem:PrizeChangeClass(playerid)
{
	new temp_string[256];
	for(new i = 1; i < CONTAINER_CLASSES_COUNT+1; i++)
	{
		format
		(
			temp_string, sizeof(temp_string),
			"%s\n\
			%s",
			temp_string,
			container_class_str[i]
		);
	}
	return ShowPlayerDialog(playerid, DIALOG_CONT_PRIZE_CHANGE_CLASS, DIALOG_STYLE_LIST,
		""cl_orange"Âûáåðèòå êëàññ ïðèçà", temp_string,
		"Âûáðàòü", "Íàçàä");
}



// ---- Îòêðûâàåò ìåíþ ðåäàêòèðîâàíèÿ òèïà êîíêðåòíîãî ïðèçà
stock ContSystem:PrizeChangeType(playerid)
{
	new temp_string[256];
	for(new i = 1; i < sizeof(container_prize_types_str); i++)
	{
		format
		(
			temp_string, sizeof(temp_string),
			"%s\n\
			%s",
			temp_string,
			container_prize_types_str[i]
		);
	}
	return ShowPlayerDialog(playerid, DIALOG_CONT_PRIZE_CHANGE_TYPE, DIALOG_STYLE_LIST,
		""cl_orange"Âûáåðèòå òèï ïðèçà", temp_string,
		"Âûáðàòü", "Íàçàä");
}



// ---- Îòêðûâàåò ìåíþ ðåäàêòèðîâàíèÿ ID (çíà÷åíèÿ) êîíêðåòíîãî ïðèçà
stock ContSystem:PrizeChangeValue(playerid, error_string[] = "")
{
	new temp_string[256];
	format
	(
		temp_string, sizeof(temp_string),
		""cl_orange"- "cl_white"Ââåäèòå íîâûé ID ïðèçà èëè êîëè÷åñòâî èãðîâûõ öåííîñòåé, âûäàâàåìûõ ïðè âûèãðûøå\n\n\
		%s",
		error_string
	);
	return ShowPlayerDialog(playerid, DIALOG_CONT_PRIZE_CHANGE_VALUE, DIALOG_STYLE_INPUT,
		""cl_orange"Èçìåíèòü ïðèç", temp_string,
		"Äàëåå", "Íàçàä");
}



// ---- Îòêðûâàåò ìåíþ èçìåíåíèÿ ãîñ. ñòîèìîñòè êîíêðåòíîãî ïðèçà
stock ContSystem:PrizeChangePrice(playerid, error_string[] = "")
{
	new temp_string[256];
	format
	(
		temp_string, sizeof(temp_string),
		""cl_orange"- "cl_white"Ââåäèòå íîâóþ ãîñ. ñòîèìîñòü äëÿ ïðèçà\n\n\
		%s",
		error_string
	);
	return ShowPlayerDialog(playerid, DIALOG_CONT_PRIZE_CHANGE_PRICE, DIALOG_STYLE_INPUT,
		""cl_orange"Èçìåíèòü ïðèç", temp_string,
		"Äàëåå", "Íàçàä");
}



// ---- Îòêðûâàåò ïîäòâåðæäåíèå óäàëåíèÿ êîíêðåòíîãî ïðèçà
stock ContSystem:PrizeDeleteAccept(playerid)
{
	return ShowPlayerDialog(playerid, DIALOG_CONT_PRIZE_DEL_ACCEPT, DIALOG_STYLE_MSGBOX,
		""cl_orange"Óäàëåíèå ïðèçà",
		""cl_orange"- "cl_white"Âû äåéñòâèòåëüíî õîòèòå óäàëèòü ïðèç?",
		"Äà", "Íåò");
}

//==============================================================================
stock ContSystem:Init()
{
	for(new i; i < MAX_CONTAINER_PRIZES; i++)
		g_prizes_data [i] = g_prizes_data_default;
	for(new i; i < MAX_CONTAINERS_COUNT; i++)
		g_container_data [i] = g_container_data_default;

	mysql_tquery(g_Handle, 
		"SELECT * FROM `containers`", "LoadContainersPublic");
}

stock ContSystem:OnPlayerKSChange(playerid, newkeys, oldkeys)
{
	if(newkeys == KEY_WALK)
	{
		SetPlayerDataEx(playerid, P_CONT_ID, ContSystem:GetPlayerContID(playerid));

		if(GetPlayerDataEx(playerid, P_CONT_ID) != -1)
		{
			switch(GetContainerData(GetPlayerDataEx(playerid, P_CONT_ID), EC_STATUS))
			{
				case CONTAINER_STATUS_AUCTION:
				{
					if(GetContainerData(GetPlayerDataEx(playerid, P_CONT_ID), EC_PLAYER_ID) == playerid)
						return SendErrorMessage(playerid, "Âû óæå ïîñòàâèëè ñòàâêó íà äàííûé êîíòåéíåð");
					else 
						return ContSystem:ShowAcceptBid(playerid, GetPlayerDataEx(playerid, P_CONT_ID));
				}
				case CONTAINER_STATUS_WINNER_WAIT:
				{
					if(playerid == GetContainerData(GetPlayerDataEx(playerid, P_CONT_ID), EC_PLAYER_ID))
						return ContSystem:OpenContainer(playerid, GetPlayerDataEx(playerid, P_CONT_ID));
				}
			}
		}
	}
	return true;
}

// - OnPlayerEditObject
stock ContSystem:OnPlayerEditObject(playerid, playerobject, objectid, response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ)
{
	if(objectid == edit_player_container_id [playerid])
	{
		switch(response)
		{
			case EDIT_RESPONSE_CANCEL:
			{
				DestroyObject(edit_player_container_id [playerid]);
				edit_player_container_id [playerid] = INVALID_OBJECT_ID;
			}
			case EDIT_RESPONSE_FINAL:
			{
				/*
					Ëîãèðîâàíèå: àäìèíèñòðàòîð ñîçäàë íîâûé êîíòåéíåð
				*/
				DestroyObject(edit_player_container_id [playerid]);
				edit_player_container_id [playerid] = INVALID_OBJECT_ID;
				ContSystem:AddNewContainer(fX, fY, fZ, fRotZ);
			}
		}
	}
	else if(objectid == GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_OBJECT_ID))
	{
		switch(response)
		{
			case EDIT_RESPONSE_CANCEL:
			{
				DestroyObject(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_OBJECT_ID));

				if(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS) == CONTAINER_STATUS_AUCTION
					|| GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS) == CONTAINER_STATUS_WINNER_WAIT
					|| GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS) == CONTAINER_STATUS_TAKE_PRIZE)
				{
					ContSystem:CreateContObject(GetPlayerDataEx(playerid, P_CONT_EDIT_ID));
				}
			}
			case EDIT_RESPONSE_FINAL:
			{
				/*
					Ëîãèðîâàíèå: àäìèíèñòðàòîð èçìåíèë ìåñòîïîëîæåíèå êîíòåéíåðà %d

					GetPlayerDataEx(playerid, P_CONT_EDIT_ID)+1
				*/

				SetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_POS_X, fX);
				SetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_POS_Y, fY);
				SetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_POS_Z, fZ);
				SetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_POS_ROT, fRotZ);
				ContSystem:UpdateContainerPos(GetPlayerDataEx(playerid, P_CONT_EDIT_ID));

				DestroyObject(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_OBJECT_ID));

				if(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS) == CONTAINER_STATUS_AUCTION
					|| GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS) == CONTAINER_STATUS_WINNER_WAIT
					|| GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS) == CONTAINER_STATUS_TAKE_PRIZE)
				{
					ContSystem:CreateContObject(GetPlayerDataEx(playerid, P_CONT_EDIT_ID));
				}
			}
		}
		DestroyDynamic3DTextLabel(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_TEXT_LABEL));
		DestroyDynamicArea(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_DYNAMIC_AREA));
		ContSystem:UpdateText(GetPlayerDataEx(playerid, P_CONT_EDIT_ID));
	}
}

stock ContSystem:SecondTimer()
{
	for(new container_id; container_id < MAX_CONTAINERS_COUNT; container_id++)
	{
		if(GetContainerData(container_id, EC_STATUS) != CONTAINER_STATUS_NONE)
		{
			switch(GetContainerData(container_id, EC_STATUS))
			{
				case CONTAINER_STATUS_AUCTION:
				{
					if(GetContainerData(container_id, EC_PLAYER_ID) != INVALID_PLAYER_ID)
					{
						AddContainerData(container_id, EC_STOP_TIMER, -, 1);
						ContSystem:UpdateText(container_id);

						if(GetContainerData(container_id, EC_STOP_TIMER) <= 0)
						{
							SendClientMessageEx(GetContainerData(container_id, EC_PLAYER_ID), -1, " "cl_orange"Ïîçäðàâëÿåì! Âû âûèãðàëè â àâóêöèîíå çà êîíòåéíåð ¹%d", container_id+1);
							SendClientMessageEx(GetContainerData(container_id, EC_PLAYER_ID), -1, " "cl_orange"Ó âàñ åñòü "cl_white"%d "cl_orange"ìèíóò(-û) ÷òîáû çàáðàòü ïðèç èç êîíòåéíåðà "cl_white"¹%d "cl_orange"èëè ïðîäàòü åãî â ôîíä ïîðòà", CONTAINER_WIN_TIME, container_id+1);
							SendClientMessageEx(GetContainerData(container_id, EC_PLAYER_ID), -1, " "cl_orange"Ïî èñòå÷åíèþ "cl_white"%d "cl_orange"ìèíóò âàø ïðèç áóäåò àâòîìàòè÷åñêè äîáàâëåí âàì", CONTAINER_WIN_TIME);
							SetContainerData(container_id, EC_STATUS, CONTAINER_STATUS_WINNER_WAIT);
							SetContainerData(container_id, EC_WAIT_TIMER, CONTAINER_WIN_TIME*60);
							ContSystem:UpdateText(container_id);
							GiveMoney(GetContainerData(container_id, EC_PLAYER_ID), -GetContainerData(container_id, EC_PRICE));

							/*
								Ëîãèðîâàíèå: èãðîê GetContainerData(container_id, EC_PLAYER_ID) âûèãðàë áîðüáó çà êîíòåéíåð conteiner_id+1
							*/
						}
						else if(!IsPlayerInRangeOfPoint(GetContainerData(container_id, EC_PLAYER_ID), 
							50.0, 
							GetContainerData(container_id, EC_POS_X),
							GetContainerData(container_id, EC_POS_Y),
							GetContainerData(container_id, EC_POS_Z)))
						{
							SendInfoMessage(GetContainerData(container_id, EC_PLAYER_ID), "Âàøà ñòàâêà çà êîíòåéíåð áûëà àííóëèðîâàíà, âû îòîøëè ñëèøêîì äàëåêî");
							SetContainerData(container_id, EC_STATUS, CONTAINER_STATUS_WAIT);
							SetContainerData(container_id, EC_PLAYER_ID, INVALID_PLAYER_ID);
							ContSystem:UpdateText(container_id);
						}
						else if(!IsPlayerConnected(GetContainerData(container_id, EC_PLAYER_ID)))
						{
							SetContainerData(container_id, EC_STATUS, CONTAINER_STATUS_WAIT);
							SetContainerData(container_id, EC_PLAYER_ID, INVALID_PLAYER_ID);
							ContSystem:UpdateText(container_id);
						}
					}
					else
					{
						if(gettime() >= GetContainerData(container_id, EC_STOP_AUCTION_TIME))
						{
							SetContainerData(container_id, EC_STATUS, CONTAINER_STATUS_WAIT);
							SetContainerData(container_id, EC_PLAYER_ID, INVALID_PLAYER_ID);
							ContSystem:DeleteContObject(container_id);
							ContSystem:UpdatePrize(container_id, true, true);
							ContSystem:UpdateText(container_id);
						}
					}
				}
				case CONTAINER_STATUS_WAIT:
				{
					ContSystem:UpdateText(container_id);
					if(!containers_freezed)
					{
						new current_hour, current_minute, current_second;
						new temp_hour, temp_minute;

						gettime(current_hour, current_minute, current_second);
						for(new g; g < sizeof(container_start_time); g++)
						{
							sscanf(container_start_time [g], "p<:>ii", temp_hour, temp_minute);
							if(current_hour == temp_hour &&
								current_minute == temp_minute && current_second == 0)
							{
								ContSystem:StartAuction(container_id);
								break;
							}
						}
					}
				}
				case CONTAINER_STATUS_WINNER_WAIT:
				{
					AddContainerData(container_id, EC_WAIT_TIMER, -, 1);
					if(GetContainerData(container_id, EC_WAIT_TIMER) <= 0)
					{
						ContSystem:GiveContainerPrize(container_id);
					}
				}
			}
		}
	}

	foreach(new playerid : Player)
	{
		if(GetPlayerDataEx(playerid, P_CONT_TAKE_PRIZE_TIME) > 0)
		{
			AddPlayerDataEx(playerid, P_CONT_TAKE_PRIZE_TIME, -, 1);
			if(GetPlayerDataEx(playerid, P_CONT_TAKE_PRIZE_TIME) <= 0)
				ContSystem:ShowTakePrizeDialog(playerid);
		}
	}
	return 1;
}

stock ContSystem:OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
		case DIALOG_CONTMENU:
		{
			if(response)
			{
				switch(listitem)
				{
					case 0: ContSystem:ShowContListMenu(playerid);
					case 1: ContSystem:EditPlayerNewCont(playerid);
					case 2: ContSystem:ShowPrizeList(playerid);
				}
			}
		}

		case DIALOG_CONTMENU_LIST:
		{
			if(!response) ContSystem:ShowContMenu(playerid);
			else
			{
				SetPlayerDataEx(playerid, P_CONT_EDIT_ID, edit_player_container_listitem [playerid][listitem]);
				ContSystem:ShowContEdit(playerid);
			}
		}

		case DIALOG_CONTMENU_EDIT:
		{
			if(!response) ContSystem:ShowContListMenu(playerid);
			else
			{
				switch(listitem)
				{
					case 0:
					{
						if(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS) != CONTAINER_STATUS_WAIT)
							SendErrorMessage(playerid, "Àóêöèîí ó äàííîãî êîíòåéíåðà óæå íà÷àò");
						else
						{
							/*
								Ëîãèðîâàíèå: àäìèíèñòðàòîð íà÷àë àóêíöèîí ó êîíòåéíåðà %d

								GetPlayerDataEx(playerid, P_CONT_EDIT_ID)+1
							*/

							SendInfoMessage(playerid, "Âû íà÷àëè àóêöèîí ó êîíòåéíåðà");
							ContSystem:StartAuction(GetPlayerDataEx(playerid, P_CONT_EDIT_ID));
						}
					}
					case 1:
					{
						if(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS) == CONTAINER_STATUS_WAIT)
						{
							SendErrorMessage(playerid, "Àóêöèîí çà äàííûé êîíòåéíåð íå íà÷àò");
						}
						else if(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS) != CONTAINER_STATUS_AUCTION
							|| GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_PLAYER_ID) != INVALID_PLAYER_ID)
						{
							SendErrorMessage(playerid, "Çà äàííûé êîíòåéíåð óæå èä¸ò áîðüáà ëèáî îí îæèäàåò îòêðûòèÿ ïîáåäèòåëåì");
						}
						else
						{
							/*
								Ëîãèðîâàíèå: àäìèíèñòðàòîð çàâåðøèë àóêíöèîí ó êîíòåéíåðà %d

								GetPlayerDataEx(playerid, P_CONT_EDIT_ID)+1
							*/

							SendInfoMessage(playerid, "Âû çàâåðøèëè àóêöèîí ó êîíòåéíåðà");
							SetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS, CONTAINER_STATUS_WAIT);
							SetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_PLAYER_ID, INVALID_PLAYER_ID);
							ContSystem:DeleteContObject(GetPlayerDataEx(playerid, P_CONT_EDIT_ID));
							ContSystem:UpdatePrize(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), true, true);
							ContSystem:UpdateText(GetPlayerDataEx(playerid, P_CONT_EDIT_ID));
							
						}
					}
					case 2:
					{
						if(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS) == CONTAINER_STATUS_NONE)
							SendErrorMessage(playerid, "Äàííîãî êîíòåéíåðà óæå íå ñóùåñòâóåò");
						else
							ContSystem:ShowEditPrizeMenu(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_ID));
					}
					case 3:
					{
						if(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS) == CONTAINER_STATUS_NONE)
							SendErrorMessage(playerid, "Äàííîãî êîíòåéíåðà óæå íå ñóùåñòâóåò");
						else if(!IsPlayerInRangeOfPoint(playerid,
							15.0, 
							GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_POS_X),
							GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_POS_Y),
							GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_POS_Z)))
						{
							SendErrorMessage(playerid, "Âû äàëåêî îò èçíà÷àëüíîãî ìåñòîïîëîæåíèÿ êîíòåéíåðà");
						}
						else if(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS) == CONTAINER_STATUS_WINNER_WAIT
							|| GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_PLAYER_ID) != INVALID_PLAYER_ID)
						{
							SendErrorMessage(playerid, "Äàííûé êîíòåéíåð ñåé÷àñ îæèäàåò ïîêà ïðèç çàáåð¸ò èãðîê èëè çà íåãî èä¸ò áîðüáà");	
						}
						else
						{
							if(IsValidObject(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_OBJECT_ID)))
							{
								ContSystem:DeleteContObject(GetPlayerDataEx(playerid, P_CONT_EDIT_ID));
							}
							GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_OBJECT_ID) = CreateObject(CONTAINER_OBJECT_ID, 
								GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_POS_X), 
								GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_POS_Y), 
								GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_POS_Z), 
								0.0,
								0.0, 
								GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_POS_ROT));
							EditObject(playerid, GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_OBJECT_ID));
						}
					}
					case 4:
					{
						if(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS) == CONTAINER_STATUS_NONE)
							SendErrorMessage(playerid, "Äàííîãî êîíòåéíåðà óæå íå ñóùåñòâóåò");
						else if(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS) == CONTAINER_STATUS_WINNER_WAIT
							|| GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_PLAYER_ID) != INVALID_PLAYER_ID)
						{
							SendErrorMessage(playerid, "Äàííûé êîíòåéíåð ñåé÷àñ îæèäàåò ïîêà ïðèç çàáåð¸ò èãðîê èëè çà íåãî èä¸ò áîðüáà");	
						}
						else
						{
							/*
								Ëîãèðîâàíèå: àäìèíèñòðàòîð óäàëèë êîíòåéíåð %d

								GetPlayerDataEx(playerid, P_CONT_EDIT_ID)+1
							*/
							SendInfoMessage(playerid, "Âû óäàëèëè êîíòåéíåð");
							ContSystem:DeleteContainer(GetPlayerDataEx(playerid, P_CONT_EDIT_ID));
						}
					}
				}
			}
		}

		case DIALOG_CONT_ACCEPT_BID:
		{
			if(response)
			{
				if(GetContainerData(GetPlayerDataEx(playerid, P_CONT_ID), EC_PLAYER_ID) != INVALID_PLAYER_ID)
				{
					if(GetPlayerDataEx(playerid, P_MONEY) < GetContainerData(GetPlayerDataEx(playerid, P_CONT_ID), EC_PRICE) + CONTAINER_BID_PRICE)
						SendErrorMessage(playerid, "Ó âàñ íå äîñòàòî÷íî ñðåäñòâ");
					else
					{
						AddContainerData(GetPlayerDataEx(playerid, P_CONT_ID), EC_PRICE, +, CONTAINER_BID_PRICE);
						SetContainerData(GetPlayerDataEx(playerid, P_CONT_ID), EC_PLAYER_ID, playerid);
						SetContainerData(GetPlayerDataEx(playerid, P_CONT_ID), EC_STOP_TIMER, CONTAINER_STOP_AUCTION_TIME);
						ContSystem:UpdateText(GetPlayerDataEx(playerid, P_CONT_ID));

						SendInfoMessage(playerid, "Âû ïîñòàâèëè ñòàâêó íà êîíòåéíåð. Íå îòõîäèòå äàëåêî îò êîíòåéíåðà, ÷òîáû ñòàâêà íå àííóëèðîâàëàñü");

						/*
							Ëîãèðîâàíèå: èãðîê ïîñòàâèë GetContainerData(GetPlayerDataEx(playerid, P_CONT_ID), EC_PRICE) + CONTAINER_BID_PRICE äîëëàðîâ íà êîíòåéíåð GetPlayerDataEx(playerid, P_CONT_ID)+1
						*/
					}
				}
				else
				{
					if(GetPlayerDataEx(playerid, P_MONEY) < GetContainerData(GetPlayerDataEx(playerid, P_CONT_ID), EC_START_PRICE))
						SendErrorMessage(playerid, "Ó âàñ íå äîñòàòî÷íî ñðåäñòâ");
					else
					{
						SetContainerData(GetPlayerDataEx(playerid, P_CONT_ID), EC_PRICE, GetContainerData(GetPlayerDataEx(playerid, P_CONT_ID), EC_START_PRICE));
						SetContainerData(GetPlayerDataEx(playerid, P_CONT_ID), EC_PLAYER_ID, playerid);
						SetContainerData(GetPlayerDataEx(playerid, P_CONT_ID), EC_STOP_TIMER, CONTAINER_STOP_AUCTION_TIME);
						ContSystem:UpdateText(GetPlayerDataEx(playerid, P_CONT_ID));

						SendInfoMessage(playerid, "Âû ïîñòàâèëè ñòàâêó íà êîíòåéíåð. Íå îòõîäèòå äàëåêî îò êîíòåéíåðà, ÷òîáû ñòàâêà íå àííóëèðîâàëàñü");

						/*
							Ëîãèðîâàíèå: èãðîê ïîñòàâèë GetContainerData(GetPlayerDataEx(playerid, P_CONT_ID), EC_START_PRICE) äîëëàðîâ íà êîíòåéíåð GetPlayerDataEx(playerid, P_CONT_ID)+1
						*/
					}
				}
			}
		}

		case DIALOG_CONTAINER_TAKE_PRIZE:
		{
			if(response)
				ContSystem:GiveContainerPrize(GetPlayerDataEx(playerid, P_TAKE_PRIZE_CONTAINER_ID));
			else
				ContSystem:SellContainerPrize(GetPlayerDataEx(playerid, P_TAKE_PRIZE_CONTAINER_ID));

			SetPlayerDataEx(playerid, P_TAKE_PRIZE_CONTAINER_ID, -1);
		}

		case DIALOG_CONTMENU_EDIT_PRIZE:
		{
			if(!response)
				ContSystem:ShowContListMenu(playerid);
			else
			{
				if(GetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_STATUS) == CONTAINER_STATUS_NONE)
					SendErrorMessage(playerid, "Äàííîãî êîíòåéíåðà óæå íå ñóùåñòâóåò");
				else
				{
					switch(listitem)
					{
						case 0: ContSystem:ShowChangeContClass(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_ID));
						case 1: ContSystem:ShowChangeContType(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_ID));
					}
				}
			}
		}

		case DIALOG_CNTM_EDIT_PRIZE_CLASS:
		{
			if(!response)
				ContSystem:ShowEditPrizeMenu(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_ID));
			else
			{
				/*
					Ëîãèðîâàíèå: àäìèíèñòðàòîð èçìåíèë êëàññ ó êîíòåéíåðà %d íà êëàññ %s

					GetPlayerDataEx(playerid, P_CONT_EDIT_ID)+1
					container_class_str [listitem+1]
				*/

				SetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_CLASS, listitem+1);
				ContSystem:UpdatePrize(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), true, true);

				ContSystem:ShowEditPrizeMenu(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_ID));
			}
		}

		case DIALOG_CNTM_EDIT_PRIZE_TYPE:
		{
			if(!response)
				ContSystem:ShowEditPrizeMenu(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_ID));
			else
			{
				/*
					Ëîãèðîâàíèå: àäìèíèñòðàòîð èçìåíèë òèï ó êîíòåéíåðà %d íà êëàññ %s

					GetPlayerDataEx(playerid, P_CONT_EDIT_ID)+1
					container_type_str [listitem+1]
				*/

				SetContainerData(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), EC_TYPE, listitem+1);
				ContSystem:UpdatePrize(GetPlayerDataEx(playerid, P_CONT_EDIT_ID), true, true);

				ContSystem:ShowEditPrizeMenu(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_ID));
			}
		}

		case DIALOG_CONTAINER_PRIZE_LIST:
		{
			if(response)
			{
				if(listitem == 0)
					ContSystem:ShowAddNewPrize(playerid);
				else
				{
					SetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE, edit_player_container_listitem [playerid][listitem-1]);
					ContSystem:ShowEditPrize(playerid, edit_player_container_listitem [playerid][listitem-1]);
				}
			}
		}

		case DIALOG_CONT_PRIZE_ADD_CLASS:
		{
			if(!response)
				ContSystem:ShowPrizeList(playerid);
			else
			{
				SetPlayerDataEx(playerid, P_ADD_PRIZE_CLASS, listitem+1);
				ContSystem:ShowAddNewPrizeType(playerid);
			}
		}

		case DIALOG_CONT_PRIZE_ADD_TYPE:
		{
			if(!response)
				ContSystem:ShowAddNewPrize(playerid);
			else
			{
				SetPlayerDataEx(playerid, P_ADD_PRIZE_TYPE, listitem+1);
				ContSystem:ShowAddNewPrizeValue(playerid);
			}
		}

		case DIALOG_CONT_PRIZE_ADD_VALUE:
		{
			if(!response)
				ContSystem:ShowAddNewPrizeType(playerid);
			else
			{
				if(GetPlayerDataEx(playerid, P_ADD_PRIZE_TYPE) < 4)
				{
					SetPlayerDataEx(playerid, P_ADD_PRIZE_VALUE, strval(inputtext));
					ContSystem:ShowAddNewPrizePrice(playerid);
				}
				else
				{
					/*
						Ëîãèðîâàíèå: àäìèíèñòðàòîð ñîçäàë íîâûé ïðèç äëÿ àóêöèîíà êîíòåéíåðîâ
					*/

					SendInfoMessage(playerid, "Âû ñîçäàëè íîâûé ïðèç äëÿ àóêöèîíà êîíòåéíåðîâ");
					ContSystem:CreateNewPrize(GetPlayerDataEx(playerid, P_ADD_PRIZE_CLASS), GetPlayerDataEx(playerid, P_ADD_PRIZE_TYPE), strval(inputtext));
				}
			}

		}

		case DIALOG_CONT_PRIZE_ADD_PRICE:
		{
			if(!response)
				ContSystem:ShowAddNewPrizeValue(playerid);
			else
			{
				if(strval(inputtext) < 0)
				{
					ContSystem:ShowAddNewPrizePrice(playerid, ""cl_grey"Êîððåêòíî óêàæèòå öåíó");
				}
				else
				{
					/*
						Ëîãèðîâàíèå: àäìèíèñòðàòîð ñîçäàë íîâûé ïðèç äëÿ àóêöèîíà êîíòåéíåðîâ
					*/
					SendInfoMessage(playerid, "Âû ñîçäàëè íîâûé ïðèç äëÿ àóêöèîíà êîíòåéíåðîâ");
					ContSystem:CreateNewPrize(GetPlayerDataEx(playerid, P_ADD_PRIZE_CLASS), GetPlayerDataEx(playerid, P_ADD_PRIZE_TYPE), GetPlayerDataEx(playerid, P_ADD_PRIZE_VALUE), strval(inputtext));
				}
			}
		}

		case DIALOG_CONTMENU_PRIZE_EDIT:
		{
			if(!response)
				ContSystem:ShowPrizeList(playerid);
			else
			{
				if(GetPrizeData(GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE), EC_PRIZE_SQL_ID) == -1)
					SendErrorMessage(playerid, "Äàííîãî ïðèçà óæå íå ñóùåñòâóåò");
				else
				{
					switch(listitem)
					{
						case 0: ContSystem:PrizeChangeClass(playerid);
						case 1: ContSystem:PrizeChangeType(playerid);
						case 2: ContSystem:PrizeChangeValue(playerid);
						case 3: ContSystem:PrizeChangePrice(playerid);
						case 4: ContSystem:PrizeDeleteAccept(playerid);
					}
				}
			}
		}

		case DIALOG_CONT_PRIZE_CHANGE_CLASS:
		{
			if(!response)
				ContSystem:ShowEditPrize(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE));
			else
			{
				if(GetPrizeData(GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE), EC_PRIZE_SQL_ID) == -1)
					SendErrorMessage(playerid, "Äàííîãî ïðèçà óæå íå ñóùåñòâóåò");
				else
				{
					SetPrizeData(GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE), EC_PRIZE_CLASS, listitem+1);
					for(new container_id; container_id < MAX_CONTAINERS_COUNT; container_id++)
					{
						if(GetContainerData(container_id, EC_TYPE) != CONTAINER_STATUS_NONE
							&& GetContainerData(container_id, EC_PRIZE) == GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE))
						{
							ContSystem:UpdatePrize(container_id);
							ContSystem:UpdateText(container_id);
						}
					}
					ContSystem:UpdatePrizeSQL(GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE));
					ContSystem:ShowEditPrize(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE));
				}
			}
		}

		case DIALOG_CONT_PRIZE_CHANGE_TYPE:
		{
			if(!response)
				ContSystem:ShowEditPrize(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE));
			else
			{
				if(GetPrizeData(GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE), EC_PRIZE_SQL_ID) == -1)
					SendErrorMessage(playerid, "Äàííîãî ïðèçà óæå íå ñóùåñòâóåò");
				else
				{
					SetPrizeData(GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE), EC_PRIZE_TYPE, listitem+1);
					for(new container_id; container_id < MAX_CONTAINERS_COUNT; container_id++)
					{
						if(GetContainerData(container_id, EC_TYPE) != CONTAINER_STATUS_NONE
							&& GetContainerData(container_id, EC_PRIZE) == GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE))
						{
							ContSystem:UpdatePrize(container_id);
							ContSystem:UpdateText(container_id);
						}
					}
					ContSystem:UpdatePrizeSQL(GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE));
					ContSystem:ShowEditPrize(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE));
				}
			}
		}

		case DIALOG_CONT_PRIZE_CHANGE_VALUE:
		{
			if(!response)
				ContSystem:ShowEditPrize(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE));
			else
			{
				if(GetPrizeData(GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE), EC_PRIZE_SQL_ID) == -1)
					SendErrorMessage(playerid, "Äàííîãî ïðèçà óæå íå ñóùåñòâóåò");
				else
				{
					SetPrizeData(GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE), EC_PRIZE_VALUE, strval(inputtext));
					ContSystem:UpdatePrizeSQL(GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE));
					ContSystem:ShowEditPrize(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE));
				}
			}
		}

		case DIALOG_CONT_PRIZE_CHANGE_PRICE:
		{
			if(!response)
				ContSystem:ShowEditPrize(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE));
			else
			{
				if(GetPrizeData(GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE), EC_PRIZE_SQL_ID) == -1)
					SendErrorMessage(playerid, "Äàííîãî ïðèçà óæå íå ñóùåñòâóåò");
				else if(strval(inputtext) <= 0)
					SendErrorMessage(playerid, "Êîððåêòíî ââåäèòå öåíó");
				else
				{
					SetPrizeData(GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE), EC_PRIZE_PRICE, strval(inputtext));
					ContSystem:UpdatePrizeSQL(GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE));
					ContSystem:ShowEditPrize(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE));
				}
			}
		}

		case DIALOG_CONT_PRIZE_DEL_ACCEPT:
		{
			if(!response)
				ContSystem:ShowEditPrize(playerid, GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE));
			else
			{
				ContSystem:DeletePrize(GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE));
				SendInfoMessage(playerid, "Âû óäàëèëè ïðèç");

				/*
					Ëîãèðîâàíèå: àäìèíèñòðàòîð %d óäàëèë ïðèç %d

					playerid
					GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE)
				*/


				for(new container_id; container_id < MAX_CONTAINERS_COUNT; container_id++)
				{
					if(GetContainerData(container_id, EC_TYPE) != CONTAINER_STATUS_NONE
						&& GetContainerData(container_id, EC_PRIZE) == GetPlayerDataEx(playerid, P_CONT_EDIT_PRIZE))
					{
						ContSystem:UpdatePrize(container_id);
						ContSystem:UpdateText(container_id);
					}
				}
			}
		}	
	}
}
