#include "Ekeko.h"
#include "ekeko_private.h"

EAPI void ekeko_init(void)
{
	eina_init();
	ekeko_document_init();
}

EAPI void ekeko_shutdown(void)
{
	eina_shutdown();
	ekeko_document_shutdown();
}

EAPI Eina_Bool ekeko_feature_has(const char *feature, const char *string)
{
	
}
