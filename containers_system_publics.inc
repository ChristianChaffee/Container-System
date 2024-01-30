
public: LoadContainersPublic()
{
	new rows;
	cache_get_row_count(rows);

	for(new i; i < rows; i++)
	{
		cache_get_value_name_int(i, "EC_SQL_ID", GetContainerData(i, EC_SQL_ID));
		cache_get_value_name_float(i, "EC_POS_X", GetContainerData(i, EC_POS_X));
		cache_get_value_name_float(i, "EC_POS_Y", GetContainerData(i, EC_POS_Y));
		cache_get_value_name_float(i, "EC_POS_Z", GetContainerData(i, EC_POS_Z));
		cache_get_value_name_float(i, "EC_POS_ROT", GetContainerData(i, EC_POS_ROT));
		cache_get_value_name_int(i, "EC_CLASS", GetContainerData(i, EC_CLASS));
		cache_get_value_name_int(i, "EC_TYPE", GetContainerData(i, EC_TYPE));

		SetContainerData(i, EC_STATUS, CONTAINER_STATUS_WAIT);
	}

	mysql_tquery(g_Handle, 
		"SELECT * FROM `container_prizes`", "LoadContainerPrizesPublic");
}

public: LoadNewContainer()
{
	new Float:temp_pos[4], temp_sql_id;
	cache_get_value_name_int(0, "EC_SQL_ID", temp_sql_id);
	cache_get_value_name_float(0, "EC_POS_X", temp_pos[0]);
	cache_get_value_name_float(0, "EC_POS_Y", temp_pos[1]);
	cache_get_value_name_float(0, "EC_POS_Z", temp_pos[2]);
	cache_get_value_name_float(0, "EC_POS_ROT", temp_pos[3]);

	for(new container_id; container_id < MAX_CONTAINERS_COUNT; container_id++)
	{
		if(GetContainerData(container_id, EC_STATUS) == CONTAINER_STATUS_NONE)
		{
			cache_get_value_name_int(0, "EC_SQL_ID", GetContainerData(container_id, EC_SQL_ID));
			cache_get_value_name_float(0, "EC_POS_X", GetContainerData(container_id, EC_POS_X));
			cache_get_value_name_float(0, "EC_POS_Y", GetContainerData(container_id, EC_POS_Y));
			cache_get_value_name_float(0, "EC_POS_Z", GetContainerData(container_id, EC_POS_Z));
			cache_get_value_name_float(0, "EC_POS_ROT", GetContainerData(container_id, EC_POS_ROT));
			cache_get_value_name_int(0, "EC_CLASS", GetContainerData(container_id, EC_CLASS));
			cache_get_value_name_int(0, "EC_TYPE", GetContainerData(container_id, EC_TYPE));

			SetContainerData(container_id, EC_STATUS, CONTAINER_STATUS_WAIT);

			ContSystem:UpdatePrize(container_id, _, _, true);
			ContSystem:UpdateText(container_id);
			break;
		}
	}
}

public: LoadContainerPrizesPublic()
{
	new rows;
	cache_get_row_count(rows);

	new temp_sql_id, temp_class, temp_type, temp_value, temp_prize_price;
	for(new i; i < rows; i++)
	{
		cache_get_value_name_int(i, "EC_PRIZE_SQL_ID", temp_sql_id);
		cache_get_value_name_int(i, "EC_PRIZE_CLASS", temp_class);
		cache_get_value_name_int(i, "EC_PRIZE_TYPE", temp_type);
		cache_get_value_name_int(i, "EC_PRIZE_VALUE", temp_value);
		cache_get_value_name_int(i, "EC_PRIZE_PRICE", temp_prize_price);

		ContSystem:AddNewPrize(temp_sql_id, temp_class, temp_type, temp_value, temp_prize_price);
	}

	for(new container_id; container_id < MAX_CONTAINERS_COUNT; container_id++)
	{
		if(GetContainerData(container_id, EC_STATUS) != CONTAINER_STATUS_NONE)
		{
			ContSystem:UpdatePrize(container_id, true, true);
			ContSystem:UpdateText(container_id);
		}
	}
}

public: LoadNewPrize()
{
	new temp_sql_id, temp_class, temp_type, temp_value, temp_prize_price;
	cache_get_value_name_int(0, "EC_PRIZE_SQL_ID", temp_sql_id);
	cache_get_value_name_int(0, "EC_PRIZE_CLASS", temp_class);
	cache_get_value_name_int(0, "EC_PRIZE_TYPE", temp_type);
	cache_get_value_name_int(0, "EC_PRIZE_VALUE", temp_value);
	cache_get_value_name_int(0, "EC_PRIZE_PRICE", temp_prize_price);

	ContSystem:AddNewPrize(temp_sql_id, temp_class, temp_type, temp_value, temp_prize_price);
}