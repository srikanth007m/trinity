#include <stdlib.h>
#include <linux/atmdev.h>
#include <linux/atm.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "maps.h"	// page_rand
#include "net.h"
#include "utils.h"	// ARRAY_SIZE
#include "compat.h"

void atmpvc_gen_sockaddr(struct sockaddr **addr, socklen_t *addrlen)
{
	struct sockaddr_atmpvc *atmpvc;

	atmpvc = malloc(sizeof(struct sockaddr_atmpvc));
	if (atmpvc == NULL)
		return;

	atmpvc->sap_family = PF_ATMPVC;
	atmpvc->sap_addr.itf = rand();
	atmpvc->sap_addr.vpi = rand();
	atmpvc->sap_addr.vci = rand();
	*addr = (struct sockaddr *) atmpvc;
	*addrlen = sizeof(struct sockaddr_atmpvc);
}

void atmsvc_gen_sockaddr(struct sockaddr **addr, socklen_t *addrlen)
{
	struct sockaddr_atmsvc *atmsvc;
	unsigned int i;

	atmsvc = malloc(sizeof(struct sockaddr_atmsvc));
	if (atmsvc == NULL)
		return;

	atmsvc->sas_family = PF_ATMSVC;
	for (i = 0; i < ATM_ESA_LEN; i++)
		atmsvc->sas_addr.prv[i] = rand();
	for (i = 0; i < ATM_E164_LEN; i++)
		atmsvc->sas_addr.pub[i] = rand();
	atmsvc->sas_addr.lij_type = rand();
	atmsvc->sas_addr.lij_id = rand();
	*addr = (struct sockaddr *) atmsvc;
	*addrlen = sizeof(struct sockaddr_atmsvc);
}

#define NR_SOL_ATM_OPTS ARRAY_SIZE(atm_opts)
static const unsigned int atm_opts[] = {
	SO_SETCLP, SO_CIRANGE, SO_ATMQOS, SO_ATMSAP, SO_ATMPVC, SO_MULTIPOINT };

void atm_setsockopt(struct sockopt *so)
{
	unsigned char val;

	so->level = SOL_ATM;

	val = rand() % NR_SOL_ATM_OPTS;
	so->optname = atm_opts[val];
}
