#include "Ekeko.h"
#include "ekeko_private.h"

EAPI void ekeko_init(void)
{
	eina_init();
	ekeko_document_init();
	ekeko_value_init();
}

EAPI void ekeko_shutdown(void)
{
	eina_shutdown();
	ekeko_document_shutdown();
	ekeko_value_shutdown();
}

EAPI Eina_Bool ekeko_feature_has(const char *feature, const char *string)
{
	
}
