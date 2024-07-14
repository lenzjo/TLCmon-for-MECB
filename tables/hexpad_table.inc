
hexpad_table
				.DB		'C', '8', '4', '0'
				.DB		'D', '9', '5', '1'
				.DB		'E', 'A', '6', '2'
				.DB		'F', 'B', '7', '3'
				.DB		'S', 'L', 'N',	$0D

hexpad_cmd_table
				.DB		"COPY", NULL, NULL, NULL, NULL
				.DB		"REGS", NULL, NULL, NULL, NULL
				.DB		"USER 4", NULL, NULL
				.DB		"USER 0", NULL, NULL
				.DB		"DUMP", NULL, NULL, NULL, NULL
				.DB		"PEEK", NULL, NULL, NULL, NULL
				.DB		"USER 5", NULL, NULL
				.DB		"USER 1", NULL, NULL
				.DB		"EXEC", NULL, NULL, NULL, NULL
				.DB		"POKE", NULL, NULL, NULL, NULL
				.DB		"USER 6", NULL, NULL
				.DB		"USER 2", NULL, NULL
				.DB		"FILL", NULL, NULL, NULL, NULL
				.DB		"UPTIME", NULL, NULL
				.DB		"USER 7", NULL, NULL
				.DB		"USER 3", NULL, NULL
				.DB		"SHIFT", NULL, NULL, NULL
				.DB		"SAVE", NULL, NULL, NULL, NULL
				.DB		"PREV",	NULL, NULL, NULL, NULL
				.DB		"CR", NULL, NULL, NULL, NULL, NULL, NULL
				.DB		NULL										; End marker
