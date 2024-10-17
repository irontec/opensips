/*
 * presence_reginfo module - Presence Handling of reg events
 *
 * Copyright (C) 2011, 2023 Carsten Bock, carsten@ng-voice.com
 *
 * This file is part of opensips, a free SIP server.
 *
 * opensips is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version
 *
 * opensips is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../parser/parse_content.h"
#include "../presence/event_list.h"
#include "presence_reginfo.h"
#include "notify_body.h"

extern int pres_reginfo_aggregate_presentities;

int reginfo_add_events(void)
{
	pres_ev_t event;

	/* constructing message-summary event */
	memset(&event, 0, sizeof(pres_ev_t));
	event.name.s = "reg";
	event.name.len = 3;

	event.content_type.s = "application/reginfo+xml";
	event.content_type.len = 23;
	event.default_expires = pres_reginfo_default_expires;
	event.type = PUBL_TYPE;
	event.req_auth = 0;
	event.evs_publ_handl = 0;
	/* modify XML body for each watcher to set the correct "version" */
	event.aux_body_processing = reginfo_body_setversion;
	event.aux_free_body= free_xml_body;
	event.etag_not_new = 0;

	if(pres_reginfo_aggregate_presentities) {
		/* aggregate XML body and free() function */
		event.agg_nbody = reginfo_agg_nbody;
		event.free_body = free_xml_body;
	}

	if(pres_add_event(&event) < 0) {
		LM_ERR("failed to add event \"reginfo\"\n");
		return -1;
	}
	return 0;
}
