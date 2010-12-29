#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_libvirt.h"
#include "standard/info.h"
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>

#define ARRAY_SIZE(A) (sizeof(A)/sizeof(A[0]))


//----------------- ZEND thread safe per request globals definition 
int le_libvirt_connection;
int le_libvirt_domain;
int le_libvirt_storagepool;
int le_libvirt_volume;
int le_libvirt_network;
int le_libvirt_host_device;
int le_libvirt_interface;
int le_libvirt_domain_snapshot;

ZEND_DECLARE_MODULE_GLOBALS(libvirt)

//static
ZEND_BEGIN_ARG_INFO_EX(arginfo_libvirt_connect, 0, 0, 0)
	ZEND_ARG_INFO(0, url)
        ZEND_ARG_INFO(0, readonly)
ZEND_END_ARG_INFO()

static function_entry libvirt_functions[] = {
     PHP_FE(libvirt_get_last_error,NULL)
     PHP_FE(libvirt_connect, arginfo_libvirt_connect)
     PHP_FE(libvirt_get_hostname, NULL)
     PHP_FE(libvirt_node_get_info,NULL)
	 PHP_FE(libvirt_get_capabilities, NULL)
	 PHP_FE(libvirt_get_max_vcpus, NULL)
	 PHP_FE(libvirt_node_get_free_memory, NULL)
	 PHP_FE(libvirt_get_type, NULL)
	 PHP_FE(libvirt_get_version, NULL)
	 PHP_FE(libvirt_get_lib_version, NULL)
	 PHP_FE(libvirt_get_uri, NULL)
     PHP_FE(libvirt_get_active_domain_count, NULL)
     PHP_FE(libvirt_get_defined_domain_count, NULL)
     PHP_FE(libvirt_get_domain_count, NULL)
	 PHP_FE(libvirt_get_domain_state_string, NULL)
     PHP_FE(libvirt_domain_lookup_by_name, NULL)
     PHP_FE(libvirt_domain_get_xml_desc, NULL)
     PHP_FE(libvirt_domain_get_info, NULL)
     PHP_FE(libvirt_domain_get_name, NULL)
     PHP_FE(libvirt_domain_get_uuid, NULL)
     PHP_FE(libvirt_domain_get_uuid_string, NULL)
     PHP_FE(libvirt_list_domains, NULL)
	 PHP_FE(libvirt_list_domain_names, NULL)
	 PHP_FE(libvirt_list_active_domain_names, NULL)
	 PHP_FE(libvirt_list_defined_domain_names, NULL)
     PHP_FE(libvirt_list_active_domains, NULL)
     PHP_FE(libvirt_list_defined_domains, NULL)
     PHP_FE(libvirt_domain_get_id, NULL)
     PHP_FE(libvirt_domain_lookup_by_id, NULL)
     PHP_FE(libvirt_domain_lookup_by_uuid, NULL)
     PHP_FE(libvirt_domain_lookup_by_uuid_string, NULL)
     PHP_FE(libvirt_domain_destroy, NULL)
     PHP_FE(libvirt_domain_create, NULL)
     PHP_FE(libvirt_domain_resume, NULL)
     PHP_FE(libvirt_domain_shutdown, NULL)
     PHP_FE(libvirt_domain_suspend, NULL)
     PHP_FE(libvirt_domain_undefine, NULL)
     PHP_FE(libvirt_domain_reboot, NULL)
     PHP_FE(libvirt_domain_define_xml, NULL)
     PHP_FE(libvirt_domain_create_xml, NULL)
     PHP_FE(libvirt_domain_memory_peek,NULL)
#if LIBVIR_VERSION_NUMBER>=7005
     PHP_FE(libvirt_domain_memory_stats,NULL)
#endif
     PHP_FE(libvirt_domain_block_stats,NULL)
     PHP_FE(libvirt_domain_interface_stats,NULL)
     PHP_FE(libvirt_version,NULL)
#ifdef HAVE_GETCONNECT
     PHP_FE(libvirt_domain_get_connect, NULL)
#endif /* HAVE_GETCONNECT */
     PHP_FE(libvirt_domain_migrate, NULL)
     PHP_FE(libvirt_domain_migrate_to_uri, NULL)
#if LIBVIR_VERSION_NUMBER>=7007     
     PHP_FE(libvirt_domain_get_job_info, NULL)
#endif     
	 PHP_FE(libvirt_domain_get_autostart, NULL)
	 PHP_FE(libvirt_domain_set_autostart, NULL)
	 PHP_FE(libvirt_domain_is_active, NULL)
     PHP_FE(libvirt_list_storagepools,NULL)
	 PHP_FE(libvirt_list_active_storagepools, NULL)
	 PHP_FE(libvirt_list_defined_storagepools, NULL)
	 PHP_FE(libvirt_get_storagepool_state_string, NULL)
	 PHP_FE(libvirt_storagepool_get_name, NULL)
     PHP_FE(libvirt_storagepool_lookup_by_name,NULL)
     PHP_FE(libvirt_storagepool_list_volumes,NULL)
	 PHP_FE(libvirt_list_active_networks, NULL)
	 PHP_FE(libvirt_list_defined_networks, NULL)
     PHP_FE(libvirt_storagepool_get_info,NULL)
	 PHP_FE(libvirt_storagepool_get_uuid_string, NULL)
	 PHP_FE(libvirt_storagepool_lookup_by_uuid_string, NULL)
	 PHP_FE(libvirt_storagepool_get_xml_desc, NULL)
	 PHP_FE(libvirt_storagepool_define_xml, NULL)
	 PHP_FE(libvirt_storagepool_undefine, NULL)
	 PHP_FE(libvirt_storagepool_create, NULL)
	 PHP_FE(libvirt_storagepool_destroy, NULL)
#ifdef HAVE_GETCONNECT
	 PHP_FE(libvirt_storagepool_get_connect, NULL)
#endif /* HAVE_GETCONNECT */
	 PHP_FE(libvirt_storagepool_is_active, NULL)
	 PHP_FE(libvirt_storagepool_get_volume_count, NULL)
	 PHP_FE(libvirt_storagepool_refresh, NULL)
	 PHP_FE(libvirt_storagepool_set_autostart, NULL)
	 PHP_FE(libvirt_storagepool_get_autostart, NULL)
	 PHP_FE(libvirt_get_storagepool_count, NULL)
	 PHP_FE(libvirt_get_active_storagepool_count, NULL)
	 PHP_FE(libvirt_get_defined_storagepool_count, NULL)
     PHP_FE(libvirt_storagevolume_lookup_by_name,NULL)
     PHP_FE(libvirt_storagevolume_get_info,NULL)
     PHP_FE(libvirt_storagevolume_get_xml_desc,NULL)
     PHP_FE(libvirt_storagevolume_create_xml,NULL)
	 PHP_FE(libvirt_storagevolume_get_path, NULL)
	 PHP_FE(libvirt_storagevolume_get_name, NULL)
	 PHP_FE(libvirt_get_storagevolume_type_string, NULL)
	 PHP_FE(libvirt_list_networks, NULL)
	 PHP_FE(libvirt_get_network_count, NULL)
	 PHP_FE(libvirt_get_active_network_count, NULL)
	 PHP_FE(libvirt_get_defined_network_count, NULL)
	 PHP_FE(libvirt_network_define_xml, NULL)
	 PHP_FE(libvirt_network_create, NULL)
	 PHP_FE(libvirt_network_destroy, NULL)
	 PHP_FE(libvirt_network_undefine, NULL)
	 PHP_FE(libvirt_network_get_autostart, NULL)
	 PHP_FE(libvirt_network_get_bridge_name, NULL)
#ifdef HAVE_GETCONNECT
	 PHP_FE(libvirt_network_get_connect, NULL)
#endif /* HAVE_GETCONNECT */
	 PHP_FE(libvirt_network_get_name, NULL)
	 PHP_FE(libvirt_network_get_uuid_string, NULL)
	 PHP_FE(libvirt_network_get_xml_desc, NULL)
	 PHP_FE(libvirt_network_is_active, NULL)
	 PHP_FE(libvirt_network_lookup_by_name, NULL)
	 PHP_FE(libvirt_network_lookup_by_uuid_string, NULL)
	 PHP_FE(libvirt_network_set_autostart, NULL)
	 PHP_FE(libvirt_domain_attach_device, NULL)
 	 PHP_FE(libvirt_domain_detach_device, NULL)
	 PHP_FE(libvirt_node_device_destroy, NULL)
	 PHP_FE(libvirt_node_device_dettach, NULL)
	 PHP_FE(libvirt_node_device_get_name, NULL)
	 PHP_FE(libvirt_node_device_get_parent, NULL)
	 PHP_FE(libvirt_node_device_get_xml_desc, NULL)
	 PHP_FE(libvirt_node_device_lookup_by_name, NULL)
	 PHP_FE(libvirt_node_device_num_of_caps, NULL)
	 PHP_FE(libvirt_node_device_reattach, NULL)
	 PHP_FE(libvirt_node_device_reset, NULL)
	 PHP_FE(libvirt_list_node_device, NULL)
	 PHP_FE(libvirt_get_node_device_count, NULL)
	 PHP_FE(libvirt_list_interfaces, NULL)
	 PHP_FE(libvirt_list_active_interfaces, NULL)
	 PHP_FE(libvirt_list_defined_interfaces, NULL)
	 PHP_FE(libvirt_get_interface_count, NULL)
	 PHP_FE(libvirt_get_active_interface_count, NULL)
	 PHP_FE(libvirt_get_defined_interface_count, NULL)
	 PHP_FE(libvirt_interface_create, NULL)
	 PHP_FE(libvirt_interface_define_xml, NULL)
#ifdef HAVE_GETCONNECT
	 PHP_FE(libvirt_interface_get_connect, NULL)
#endif /* HAVE_GETCONNECT */
	 PHP_FE(libvirt_interface_get_mac_string, NULL)
	 PHP_FE(libvirt_interface_get_name, NULL)
	 PHP_FE(libvirt_interface_get_xml_desc, NULL)
	 PHP_FE(libvirt_interface_lookup_by_mac_string, NULL)
	 PHP_FE(libvirt_interface_lookup_by_name, NULL)
	 PHP_FE(libvirt_interface_undefine, NULL)
	 PHP_FE(libvirt_interface_is_active, NULL)
	 PHP_FE(libvirt_domain_has_current_snapshot, NULL)
	 PHP_FE(libvirt_domain_revert_to_snapshot, NULL)
	 PHP_FE(libvirt_domain_snapshot_create_xml, NULL)
	 PHP_FE(libvirt_domain_snapshot_current, NULL)
	 PHP_FE(libvirt_domain_snapshot_delete, NULL)
	 PHP_FE(libvirt_domain_snapshot_get_xml_desc, NULL)
	 PHP_FE(libvirt_domain_snapshot_list, NULL)
	 PHP_FE(libvirt_domain_snapshot_lookup_by_name, NULL)
	 PHP_FE(libvirt_domain_snapshot_count, NULL)
     {NULL, NULL, NULL}
};


//----------------- ZEND module basic definition 
zend_module_entry libvirt_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_LIBVIRT_WORLD_EXTNAME,
    libvirt_functions,
    PHP_MINIT(libvirt),
    PHP_MSHUTDOWN(libvirt),
    PHP_RINIT(libvirt),
    PHP_RSHUTDOWN(libvirt),
    PHP_MINFO(libvirt),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_LIBVIRT_WORLD_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_LIBVIRT
ZEND_GET_MODULE(libvirt)
#endif

//----------------- PHP init options
PHP_INI_BEGIN()
STD_PHP_INI_ENTRY("libvirt.longlong_to_string", "1", PHP_INI_ALL, OnUpdateBool, longlong_to_string_ini, zend_libvirt_globals, libvirt_globals)
PHP_INI_END()

//PHP requires to have this function defined
static void php_libvirt_init_globals(zend_libvirt_globals *libvirt_globals)
{
    libvirt_globals->longlong_to_string_ini = 1;
}

//PHP request initialization
PHP_RINIT_FUNCTION(libvirt)
{
    LIBVIRT_G (last_error)=NULL;
    return SUCCESS;
}

//PHP request destruction
PHP_RSHUTDOWN_FUNCTION(libvirt)
{
    if (LIBVIRT_G (last_error)!=NULL) efree(LIBVIRT_G (last_error));
    return SUCCESS;
}

//phpinfo()
PHP_MINFO_FUNCTION(libvirt)
{
	unsigned long libVer;
	unsigned long typeVer;
	char *version;
        php_info_print_table_start();
        php_info_print_table_row(2, "Libvirt support", "enabled");
	php_info_print_table_row(2, "Extension version", PHP_LIBVIRT_WORLD_VERSION);

	if (virGetVersion	(&libVer,NULL,&typeVer)== 0)
	{
		version=emalloc(100);
		snprintf(version, 100, "%i.%i.%i", (long)((libVer/1000000) % 1000),(long)((libVer/1000) % 1000),(long)(libVer % 1000));
		php_info_print_table_row(2, "Libvirt version", version);
	}

        php_info_print_table_end();
}

//calback error for receiving errors from libvirt. Pass them to PHP and stores for last_error function
void catch_error(void *userData, virErrorPtr error)
{
	  php_error_docref(NULL TSRMLS_CC, E_WARNING,"%s",error->message);
	  if (LIBVIRT_G (last_error)!=NULL) efree(LIBVIRT_G (last_error));
	 LIBVIRT_G (last_error)=estrndup(error->message,strlen(error->message));
}


//Destructor for connection resource
static void php_libvirt_connection_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    php_libvirt_connection *conn = (php_libvirt_connection*)rsrc->ptr;
    int rv;
    rv = virConnectClose(conn->conn);
    if (rv!=0)
        php_error_docref(NULL TSRMLS_CC, E_WARNING,"virConnectClose failed with %d on destructor",rv);
    conn->conn=NULL;
}

//Destructor for domain resource
static void php_libvirt_domain_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	php_libvirt_domain *domain = (php_libvirt_domain*)rsrc->ptr;
        int rv;
	rv = virDomainFree (domain->domain);
	if (rv != 0) { php_error_docref(NULL TSRMLS_CC, E_WARNING,"virDomainFree failed with %d on destructor",rv); }
	domain->domain=NULL;
}

//Destructor for storagepool resource
static void php_libvirt_storagepool_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	php_libvirt_storagepool *pool = (php_libvirt_storagepool*)rsrc->ptr;
	if (pool != NULL)
	{
		if (pool->pool != NULL)
		{
			virStoragePoolFree (pool->pool);
		}
		pool->pool=NULL;
		efree(pool);
	}

}

static void php_libvirt_network_dtor (zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	php_libvirt_network *network = (php_libvirt_network *)rsrc->ptr;
	if (network != NULL)
	{
		if (network->network != NULL)
		{
			virNetworkFree (network->network);
		}
		network->network = NULL;
		efree (network);
	}
}

static void php_libvirt_node_device_dtor (zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	php_libvirt_node_device *device = (php_libvirt_node_device *)rsrc->ptr;
	if (device)
	{
		if (device->device)
		{
			virNodeDeviceFree (device->device);
		}
		device->device = NULL;
		efree (device);
	}
}

//Destructor for volume resource
static void php_libvirt_volume_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	php_libvirt_volume *volume = (php_libvirt_volume*)rsrc->ptr;
	if (volume != NULL)
	{
		if (volume->volume != NULL)
		{
			virStorageVolFree (volume->volume);
		}
		volume->volume=NULL;
		efree(volume);
	}
}

static void php_libvirt_interface_dtor (zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	php_libvirt_interface *interface = (php_libvirt_interface *)rsrc->ptr;
	if (interface)
	{
		if (interface->interface)
		{
			virInterfaceFree (interface->interface);
		}
		interface->interface = NULL;
		efree (interface);
	}
}

static void php_libvirt_domain_snapshot_dtor (zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	php_libvirt_domain_snapshot *snapshot = (php_libvirt_domain_snapshot *)rsrc->ptr;
	if (snapshot)
	{
		if (snapshot->snapshot)
		{
			virDomainSnapshotFree (snapshot->snapshot);
		}
		snapshot->snapshot = NULL;
		efree (snapshot);
	}
}

//ZEND Module inicialization function
PHP_MINIT_FUNCTION(libvirt)
{
    le_libvirt_connection = zend_register_list_destructors_ex(php_libvirt_connection_dtor, NULL, PHP_LIBVIRT_CONNECTION_RES_NAME, module_number);
    le_libvirt_domain = zend_register_list_destructors_ex(php_libvirt_domain_dtor, NULL, PHP_LIBVIRT_DOMAIN_RES_NAME, module_number);  //register resource types and theis descriptors
    le_libvirt_storagepool = zend_register_list_destructors_ex(php_libvirt_storagepool_dtor, NULL, PHP_LIBVIRT_STORAGEPOOL_RES_NAME, module_number);
    le_libvirt_volume = zend_register_list_destructors_ex(php_libvirt_volume_dtor, NULL, PHP_LIBVIRT_VOLUME_RES_NAME, module_number);
	le_libvirt_network = zend_register_list_destructors_ex(php_libvirt_network_dtor, NULL, PHP_LIBVIRT_NETWORK_RES_NAME, module_number);
	le_libvirt_host_device = zend_register_list_destructors_ex(php_libvirt_node_device_dtor, NULL, PHP_LIBVIRT_HOST_DEVICE_RES_NAME, module_number);
	le_libvirt_interface = zend_register_list_destructors_ex(php_libvirt_interface_dtor, NULL, PHP_LIBVIRT_INTERFACE_RES_NAME, module_number);
	le_libvirt_domain_snapshot = zend_register_list_destructors_ex(php_libvirt_domain_snapshot_dtor, NULL, PHP_LIBVIRT_INTERFACE_RES_NAME, module_number);

    ZEND_INIT_MODULE_GLOBALS(libvirt, php_libvirt_init_globals, NULL);

    //LIBVIRT CONSTANTS
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_XML_SECURE", 1, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_XML_INACTIVE", 2, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_XML_UPDATE_CPU", 4, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_NOSTATE", 0, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_RUNNING", 1, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_BLOCKED", 2, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_PAUSED", 3, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_SHUTDOWN", 4, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_SHUTOFF", 5, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_CRASHED", 6, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("VIR_MEMORY_VIRTUAL	", 1, CONST_CS | CONST_PERSISTENT);

    REGISTER_LONG_CONSTANT("VIR_CRED_USERNAME",1, CONST_CS | CONST_PERSISTENT);//	 : Identity to act as
    REGISTER_LONG_CONSTANT("VIR_CRED_AUTHNAME",2,CONST_CS | CONST_PERSISTENT);//	: Identify to authorize as
    REGISTER_LONG_CONSTANT("VIR_CRED_LANGUAGE",3, CONST_CS | CONST_PERSISTENT);//	: RFC 1766 languages, comma separated
    REGISTER_LONG_CONSTANT("VIR_CRED_CNONCE",4	, CONST_CS | CONST_PERSISTENT);//: client supplies a nonce
    REGISTER_LONG_CONSTANT("VIR_CRED_PASSPHRASE",5, CONST_CS | CONST_PERSISTENT);//	: Passphrase secret
    REGISTER_LONG_CONSTANT("VIR_CRED_ECHOPROMPT",6, CONST_CS | CONST_PERSISTENT);//	: Challenge response
    REGISTER_LONG_CONSTANT("VIR_CRED_NOECHOPROMP",7, CONST_CS | CONST_PERSISTENT);//	: Challenge response
    REGISTER_LONG_CONSTANT("VIR_CRED_REALM",8, CONST_CS | CONST_PERSISTENT);//	: Authentication realm
    REGISTER_LONG_CONSTANT("VIR_CRED_EXTERNAL",9, CONST_CS | CONST_PERSISTENT);//	: Externally managed credential More may be added - expect the unexpected

    REGISTER_LONG_CONSTANT("VIR_DOMAIN_MEMORY_STAT_SWAP_IN",0,CONST_CS | CONST_PERSISTENT);//	 : The total amount of memory written out to swap space (in kB).
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_MEMORY_STAT_SWAP_OUT",1, CONST_CS | CONST_PERSISTENT);//	: * Page faults occur when a process makes a valid access to virtual memory * that is not available. When servicing the page fault, if disk IO is * required, it is considered a major fault. If not, it is a minor fault. * These are expressed as the number of faults that have occurred. *
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_MEMORY_STAT_MAJOR_FAULT",2, CONST_CS | CONST_PERSISTENT);//
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_MEMORY_STAT_MINOR_FAULT",3,CONST_CS | CONST_PERSISTENT);//	: * The amount of memory left completely unused by the system. Memory that * is available but used for reclaimable caches should NOT be reported as * free. This value is expressed in kB. *
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_MEMORY_STAT_UNUSED",4,CONST_CS | CONST_PERSISTENT);//	: * The total amount of usable memory as seen by the domain. This value * may be less than the amount of memory assigned to the domain if a * balloon driver is in use or if the guest OS does not initialize all * assigned pages. This value is expressed in kB. *
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_MEMORY_STAT_AVAILABLE",5,CONST_CS | CONST_PERSISTENT);//	: * The number of statistics supported by this version of the interface. * To add new statistics, add them to the enum and increase this value. *
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_MEMORY_STAT_NR",6,CONST_CS | CONST_PERSISTENT);//
    
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_JOB_NONE",0,CONST_CS | CONST_PERSISTENT);	 // No job is active
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_JOB_BOUNDED",1,CONST_CS | CONST_PERSISTENT);	//Job with a finite completion time
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_JOB_UNBOUNDED",2,CONST_CS | CONST_PERSISTENT);	// Job without a finite completion time
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_JOB_COMPLETED",	3,CONST_CS | CONST_PERSISTENT);	// Job has finished, but isn't cleaned up
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_JOB_FAILED",4,CONST_CS | CONST_PERSISTENT);	//Job hit error, but isn't cleaned up
    REGISTER_LONG_CONSTANT("VIR_DOMAIN_JOB_CANCELLED",5,CONST_CS | CONST_PERSISTENT);	// Job was aborted, but isn't cleaned up
    
    REGISTER_LONG_CONSTANT("VIR_MIGRATE_LIVE",1,CONST_CS | CONST_PERSISTENT);	// 	  live migration
    REGISTER_LONG_CONSTANT("VIR_MIGRATE_PEER2PEER",2,CONST_CS | CONST_PERSISTENT);	// 	 direct source -> dest host control channel Note the less-common spelling that we're stuck with: VIR_MIGRATE_TUNNELLED should be VIR_MIGRATE_TUNNELED
    REGISTER_LONG_CONSTANT("VIR_MIGRATE_TUNNELLED",4,CONST_CS | CONST_PERSISTENT);	// 	 tunnel migration data over libvirtd connection
    REGISTER_LONG_CONSTANT("VIR_MIGRATE_PERSIST_DEST",8,CONST_CS | CONST_PERSISTENT);	// 	 persist the VM on the destination
    REGISTER_LONG_CONSTANT("VIR_MIGRATE_UNDEFINE_SOURCE",16,CONST_CS | CONST_PERSISTENT);	// 	 undefine the VM on the source
    REGISTER_LONG_CONSTANT("VIR_MIGRATE_PAUSED",32,CONST_CS | CONST_PERSISTENT);	// 	 pause on remote side
    REGISTER_LONG_CONSTANT("VIR_MIGRATE_NON_SHARED_DISK",64,CONST_CS | CONST_PERSISTENT);	// 	 migration with non-shared storage with full disk copy
    REGISTER_LONG_CONSTANT("VIR_MIGRATE_NON_SHARED_INC",128,CONST_CS | CONST_PERSISTENT);	// 	 migration with non-shared storage with incremental copy (same base image shared between source and destination)

	REGISTER_LONG_CONSTANT("VIR_DOMAIN_DEVICE_MODIFY_CURRENT", 0, CONST_CS | CONST_PERSISTENT);	//Modify device allocation based on current domain state
	REGISTER_LONG_CONSTANT("VIR_DOMAIN_DEVICE_MODIFY_LIVE", 1, CONST_CS | CONST_PERSISTENT);	//Modify live device allocation
	REGISTER_LONG_CONSTANT("VIR_DOMAIN_DEVICE_MODIFY_CONFIG", 2, CONST_CS | CONST_PERSISTENT);	//Modify persisted device allocation
	REGISTER_LONG_CONSTANT("VIR_DOMAIN_DEVICE_MODIFY_CONFIG", 4, CONST_CS | CONST_PERSISTENT);	//Forcibly modify device (ex. force eject a cdrom)

	REGISTER_LONG_CONSTANT("VIR_DOMAIN_SNAPSHOT_DELETE_CHILEREN", 1, CONST_CS | CONST_PERSISTENT);

    
    REGISTER_INI_ENTRIES();

    //Initialize libvirt and set up error callback
    virInitialize();
    virSetErrorFunc(NULL, catch_error);

    return SUCCESS;
}

//ZEND module destruction
PHP_MSHUTDOWN_FUNCTION(libvirt)
{
    UNREGISTER_INI_ENTRIES();

    //return error callback back to default (outouts to STDOUT)
    virSetErrorFunc(NULL, NULL);
    return SUCCESS;
}

//Macros for obtaining resources from arguments
#define GET_CONNECTION_FROM_ARGS(args, ...) \
	  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, args, __VA_ARGS__) == FAILURE) {\
		  RETURN_FALSE;\
	  }\
\
	  ZEND_FETCH_RESOURCE(conn, php_libvirt_connection*, &zconn, -1, PHP_LIBVIRT_CONNECTION_RES_NAME, le_libvirt_connection);\
	  if ((conn==NULL) || (conn->conn==NULL)) RETURN_FALSE;\


#define GET_DOMAIN_FROM_ARGS(args, ...) \
	  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, args, __VA_ARGS__) == FAILURE) {\
		  RETURN_FALSE;\
	  }\
\
	  ZEND_FETCH_RESOURCE(domain, php_libvirt_domain*, &zdomain, -1, PHP_LIBVIRT_DOMAIN_RES_NAME, le_libvirt_domain);\
	 if ((domain==NULL) || (domain->domain==NULL)) RETURN_FALSE;\

#define GET_STORAGEPOOL_FROM_ARGS(args, ...) \
	  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, args, __VA_ARGS__) == FAILURE) {\
		  RETURN_FALSE;\
	  }\
\
	  ZEND_FETCH_RESOURCE(pool, php_libvirt_storagepool*, &zpool, -1, PHP_LIBVIRT_STORAGEPOOL_RES_NAME, le_libvirt_storagepool);\
	 if ((pool==NULL) || (pool->pool==NULL)) RETURN_FALSE;\

#define GET_VOLUME_FROM_ARGS(args, ...) \
	  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, args, __VA_ARGS__) == FAILURE) {\
		  RETURN_FALSE;\
	  }\
\
	  ZEND_FETCH_RESOURCE(volume, php_libvirt_volume*, &zvolume, -1, PHP_LIBVIRT_VOLUME_RES_NAME, le_libvirt_volume);\
	 if ((volume==NULL) || (volume->volume==NULL)) RETURN_FALSE;\

#define GET_NETWORK_FROM_ARGS(args, ...) \
	  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, args, __VA_ARGS__) == FAILURE) {\
		  RETURN_FALSE;\
	  }\
\
	  ZEND_FETCH_RESOURCE(network, php_libvirt_network*, &znetwork, -1, PHP_LIBVIRT_NETWORK_RES_NAME, le_libvirt_network);\
	 if ((network == NULL) || (network->network == NULL)) RETURN_FALSE; \

#define GET_DEVICE_FROM_ARGS(args, ...) \
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, args, __VA_ARGS__) == FAILURE) {\
		RETURN_FALSE;\
	}\
\
	ZEND_FETCH_RESOURCE(device, php_libvirt_node_device*, &zdevice, -1, PHP_LIBVIRT_HOST_DEVICE_RES_NAME, le_libvirt_host_device);\
	if ((device == NULL) || (device->device == NULL)) RETURN_FALSE;

#define GET_INTERFACE_FROM_ARGS(args, ...) \
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, args, __VA_ARGS__) == FAILURE) {\
		RETURN_FALSE;\
	}\
\
	ZEND_FETCH_RESOURCE(interface, php_libvirt_interface*, &zinterface, -1, PHP_LIBVIRT_INTERFACE_RES_NAME, le_libvirt_interface);\
	if ((interface == NULL) || (interface->interface == NULL)) RETURN_FALSE;

#define GET_SNAPSHOT_FROM_ARGS(args, ...) \
	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, args, __VA_ARGS__) == FAILURE) {\
		RETURN_FALSE;\
	}\
\
	ZEND_FETCH_RESOURCE(snapshot, php_libvirt_domain_snapshot*, &zsnapshot, -1, PHP_LIBVIRT_DOMAIN_SNAPSHOT_RES_NAME, le_libvirt_domain_snapshot);\
	if ((snapshot == NULL) || (snapshot->snapshot == NULL)) RETURN_FALSE;

//Macro to "recreate" string with emalloc and free the original one
#define RECREATE_STRING_WITH_E(str_out, str_in) \
str_out = estrndup(str_in, strlen(str_in)); \
	 free(str_in);	 \


#define LONGLONG_INIT \
	char tmpnumber[64];

#define RETURN_LONGLONG(val) \
	if (LIBVIRT_G(longlong_to_string_ini)) { \
		char *_llv = emalloc(64); \
		snprintf(_llv,63, "%llu", (long long)val); \
		RETURN_STRING (_llv, 0); \
	} else { \
		RETURN_LONG (val); \
	}

#define LONGLONG_ASSOC(out,key,in) \
	if (LIBVIRT_G(longlong_to_string_ini)) { \
	  snprintf(tmpnumber,63,"%llu",in); \
          add_assoc_string(out,key,tmpnumber,1); \
        } \
	else \
	{ \
	   add_assoc_long(out,key,in); \
	}

#define LONGLONG_INDEX(out,key,in) \
	if (LIBVIRT_G(longlong_to_string_ini)) { \
	  snprintf(tmpnumber,63,"%llu",in); \
          add_index_string(out,key,tmpnumber,1); \
        } \
	else \
	{ \
           add_index_long(out, key,in); \
	}




//Authentication callback function. Should receive list of credentials via cbdata and pass the requested one to libvirt	 
static int libvirt_virConnectAuthCallback(virConnectCredentialPtr cred,  unsigned int ncred,  void *cbdata)
{
	int i,j;
	php_libvirt_cred_value *creds=(php_libvirt_cred_value*) cbdata;
	for(i=0;i<ncred;i++)
	{
		//printf ("Cred %i: type %i, prompt %s challenge %s\n",i,cred[i].type,cred[i].prompt,cred[i].challenge);
		if (creds != NULL)
			for (j=0;j<creds[0].count;j++)
			{
				if (creds[j].type==cred[i].type)
						{
							cred[i].resultlen=creds[j].resultlen;
							cred[i].result=malloc(creds[j].resultlen);
							strncpy(cred[i].result,creds[j].result,creds[j].resultlen);
						}
			}
			//printf ("Result: %s (%i)\n",cred[i].result,cred[i].resultlen);
	}

	return 0;
}

static int libvirt_virConnectCredType[] = {
    VIR_CRED_AUTHNAME,
    VIR_CRED_ECHOPROMPT,
    VIR_CRED_REALM,
    VIR_CRED_PASSPHRASE,
    VIR_CRED_NOECHOPROMPT,
//    VIR_CRED_EXTERNAL,
};



	 

PHP_FUNCTION(libvirt_connect)
{
    php_libvirt_connection *conn;
    php_libvirt_cred_value *creds=NULL;
    zval* zcreds=NULL;
    zval **data;
    int i;
    int j;
    int credscount=0;
    
    virConnectAuth libvirt_virConnectAuth= {  	libvirt_virConnectCredType, sizeof(libvirt_virConnectCredType)/sizeof(int), libvirt_virConnectAuthCallback,NULL};
    
    char *url=NULL;
    int url_len=0;
    int readonly=1;
    
	HashTable *arr_hash;
	HashPosition pointer;
	int array_count;
	
	char *key;
	unsigned int key_len;
	unsigned long index;
	
	unsigned long libVer;
	unsigned long typeVer;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|sba", &url,&url_len,&readonly,&zcreds) == FAILURE) {
        RETURN_FALSE;
    }

    if (virGetVersion	(&libVer,NULL,&typeVer)!= 0)
		RETURN_FALSE;
	
	
   if (libVer<6002)
   {
	   php_error_docref(NULL TSRMLS_CC, E_WARNING,"Only libvirt 0.6.2 and higher supported. Please upgrade your libvirt.");
	   if (LIBVIRT_G (last_error)!=NULL) efree(LIBVIRT_G (last_error));
	   LIBVIRT_G (last_error)=estrdup("Only libvirt 0.6.2 and higher supported. Please upgrade your libvirt.");
	   RETURN_FALSE;
   }

    conn=emalloc(sizeof(php_libvirt_connection));
    if (zcreds==NULL)
    {	//connecting without providing authentication
	    if (readonly)
		{
			conn->conn = virConnectOpenReadOnly(url);
		}
		else
		{
			conn->conn = virConnectOpen(url);
		}
    }
    else
    {  //connecting with authentication (using callback)
	    
	    arr_hash = Z_ARRVAL_P(zcreds);
	    array_count = zend_hash_num_elements(arr_hash);
	    
	    credscount=array_count;
	   creds=emalloc(credscount*sizeof(php_libvirt_cred_value));
	   j=0;
	    //parse the input Array and create list of credentials. The list (array) is passed to callback function.
	    for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer); zend_hash_get_current_data_ex(arr_hash, (void**) &data, &pointer) == SUCCESS; zend_hash_move_forward_ex(arr_hash, &pointer)) {
		    	if (Z_TYPE_PP(data) == IS_STRING) {
				
					if (zend_hash_get_current_key_ex(arr_hash, &key, &key_len, &index, 0, &pointer) == HASH_KEY_IS_STRING) {
						PHPWRITE(key, key_len);
					} else {
						creds[j].type=index;
						creds[j].result=emalloc(Z_STRLEN_PP(data));
						creds[j].resultlen=Z_STRLEN_PP(data);
						strncpy(creds[j].result,Z_STRVAL_PP(data),Z_STRLEN_PP(data));
						j++;
					}
				}
	    }
	   creds[0].count=j;
	   libvirt_virConnectAuth.cbdata= (void*)creds ;
	   conn->conn= virConnectOpenAuth (url, &libvirt_virConnectAuth, readonly ? VIR_CONNECT_RO : 0);
	   for (i=0;i<creds[0].count;i++) efree(creds[i].result);
	  efree(creds);
    }

    if (conn->conn == NULL) RETURN_FALSE;
    ZEND_REGISTER_RESOURCE(return_value, conn, le_libvirt_connection);
    conn->resource_id=Z_LVAL_P(return_value);
} 


PHP_FUNCTION(libvirt_get_hostname)
{
	//char *	virConnectGetHostname		(virConnectPtr conn);
	  php_libvirt_connection *conn=NULL;
	  zval *zconn;
	  char *hostname;
	  char *hostname_out;

	  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zconn) == FAILURE) {
		  RETURN_FALSE;
	  }

	  ZEND_FETCH_RESOURCE(conn, php_libvirt_connection*, &zconn, -1, PHP_LIBVIRT_CONNECTION_RES_NAME, le_libvirt_connection);
	  if ((conn==NULL) || (conn->conn==NULL)) RETURN_FALSE;

	  if (conn==NULL) RETURN_FALSE;
	  if (conn->conn==NULL) RETURN_FALSE;
	  
	  hostname=virConnectGetHostname(conn->conn);
	  if (hostname==NULL) RETURN_FALSE;
	  
	  RECREATE_STRING_WITH_E(hostname_out,hostname);
	  
	  RETURN_STRING(hostname_out,0);
}

PHP_FUNCTION(libvirt_node_get_info)
{
	  virNodeInfo info;
	  php_libvirt_connection *conn=NULL;
	  zval *zconn;
	  int retval;


	 GET_CONNECTION_FROM_ARGS("r",&zconn);


	  retval=virNodeGetInfo	(conn->conn,&info);
	  if (retval==-1) RETURN_FALSE;

	 array_init(return_value);
	 add_assoc_string(return_value, "model", info.model, 1);
	 add_assoc_long(return_value, "memory", (long)info.memory);
	 add_assoc_long(return_value, "cpus", (long)info.cpus);
	 add_assoc_long(return_value, "nodes", (long)info.nodes);
	 add_assoc_long(return_value, "sockets", (long)info.sockets);
	 add_assoc_long(return_value, "cores", (long)info.cores);
	 add_assoc_long(return_value, "threads", (long)info.threads);
	 add_assoc_long(return_value, "mhz", (long)info.mhz);



}

PHP_FUNCTION(libvirt_get_active_domain_count)
{

	 php_libvirt_connection *conn=NULL;
	 zval *zconn;
	 int count;

	 GET_CONNECTION_FROM_ARGS("r",&zconn);
	  
	 count=virConnectNumOfDomains (conn->conn);
	 if (count <0) RETURN_FALSE;
	 RETURN_LONG(count);
}

PHP_FUNCTION(libvirt_get_defined_domain_count)
{

	 php_libvirt_connection *conn=NULL;
	 zval *zconn;
	 int count;

	 GET_CONNECTION_FROM_ARGS("r",&zconn);
	  
	 count=virConnectNumOfDefinedDomains (conn->conn);
	 if (count <0) RETURN_FALSE;
	 RETURN_LONG(count);
}

PHP_FUNCTION(libvirt_get_domain_count)
{

	 php_libvirt_connection *conn=NULL;
	 zval *zconn;
	 int count_defined;
	 int count_active;

	GET_CONNECTION_FROM_ARGS("r",&zconn);
	  
	 count_defined=virConnectNumOfDefinedDomains (conn->conn);
	 if (count_defined <0) RETURN_FALSE;
	 count_active=virConnectNumOfDomains (conn->conn);
	 if (count_active <0) RETURN_FALSE;
	 RETURN_LONG(count_defined+count_active);
}


PHP_FUNCTION(libvirt_domain_lookup_by_name)
{

	 php_libvirt_connection *conn=NULL;
	 zval *zconn;
	 int name_len;
	 char *name=NULL;
	 virDomainPtr domain=NULL;
	 php_libvirt_domain *res_domain;

	 GET_CONNECTION_FROM_ARGS("rs",&zconn,&name,&name_len);
	 
	 if ( (name == NULL) || (name_len<1)) RETURN_FALSE;
	 domain=virDomainLookupByName	(conn->conn,name);
	 if (domain==NULL) RETURN_FALSE;
	 
	 res_domain= emalloc(sizeof(php_libvirt_domain));
	 res_domain->domain = domain;

         res_domain->conn= conn;

	 ZEND_REGISTER_RESOURCE(return_value, res_domain, le_libvirt_domain);
}

PHP_FUNCTION(libvirt_domain_lookup_by_uuid)
{

	 php_libvirt_connection *conn=NULL;
	 zval *zconn;
	 int uuid_len;
	 unsigned char *uuid=NULL;
	 virDomainPtr domain=NULL;
	 php_libvirt_domain *res_domain;

	 GET_CONNECTION_FROM_ARGS("rs",&zconn,&uuid,&uuid_len);
	 
	 if ( (uuid == NULL) || (uuid_len<1)) RETURN_FALSE;
	 domain=virDomainLookupByUUID	(conn->conn,uuid);
	 if (domain==NULL) RETURN_FALSE;
	 
	 res_domain= emalloc(sizeof(php_libvirt_domain));
	 res_domain->domain = domain;

	 res_domain->conn=conn;


	 ZEND_REGISTER_RESOURCE(return_value, res_domain, le_libvirt_domain);
}

PHP_FUNCTION(libvirt_domain_lookup_by_uuid_string)
{

	 php_libvirt_connection *conn=NULL;
	 zval *zconn;
	 int uuid_len;
	 char *uuid=NULL;
	 virDomainPtr domain=NULL;
	 php_libvirt_domain *res_domain;

	 GET_CONNECTION_FROM_ARGS("rs",&zconn,&uuid,&uuid_len);
	 
	 if ( (uuid == NULL) || (uuid_len<1)) RETURN_FALSE;
	 domain=virDomainLookupByUUIDString	(conn->conn,uuid);
	 if (domain==NULL) RETURN_FALSE;
	 
	 res_domain= emalloc(sizeof(php_libvirt_domain));
	 res_domain->domain = domain;

	 res_domain->conn=conn;

	 ZEND_REGISTER_RESOURCE(return_value, res_domain, le_libvirt_domain);
}

PHP_FUNCTION(libvirt_domain_lookup_by_id)
{

	 php_libvirt_connection *conn=NULL;
	 zval *zconn;
	 long id;
	 virDomainPtr domain=NULL;
	 php_libvirt_domain *res_domain;

	 GET_CONNECTION_FROM_ARGS("rl",&zconn,&id);
	 	 
	 domain=virDomainLookupByID	(conn->conn,(int)id);
	 if (domain==NULL) RETURN_FALSE;
	 
	 res_domain= emalloc(sizeof(php_libvirt_domain));
	 res_domain->domain = domain;
	 res_domain->conn=conn;


	 ZEND_REGISTER_RESOURCE(return_value, res_domain, le_libvirt_domain);
}


PHP_FUNCTION(libvirt_get_last_error)
{
	if (LIBVIRT_G (last_error) == NULL) RETURN_NULL();
	  RETURN_STRING(LIBVIRT_G (last_error),1);
}


PHP_FUNCTION(libvirt_storagepool_lookup_by_name)
{

         php_libvirt_connection *conn=NULL;
         zval *zconn;
         //zval *zpool;
         int name_len;
         char *name=NULL;
	 virStoragePoolPtr pool=NULL;
         php_libvirt_storagepool *res_pool;

         GET_CONNECTION_FROM_ARGS("rs",&zconn,&name,&name_len);

         if ( (name == NULL) || (name_len<1)) RETURN_FALSE;
         pool=virStoragePoolLookupByName   (conn->conn,name);
         if (pool==NULL) RETURN_FALSE;

         res_pool = emalloc(sizeof(php_libvirt_storagepool));
         res_pool->pool = pool;
		 res_pool->conn = conn;

         ZEND_REGISTER_RESOURCE(return_value, res_pool, le_libvirt_storagepool);
}

PHP_FUNCTION(libvirt_storagepool_list_volumes)
{
        //php_libvirt_connection *conn=NULL;
        php_libvirt_storagepool *pool=NULL;
        //zval *zconn;
        zval *zpool;
        char **names=NULL;
	int expectedcount=-1;
	int i;
	int count=-1;

        GET_STORAGEPOOL_FROM_ARGS("r",&zpool);

	expectedcount=virStoragePoolNumOfVolumes (pool->pool);

	names=emalloc(expectedcount*sizeof(char *));

        count=virStoragePoolListVolumes(pool->pool,names,expectedcount);
	array_init(return_value);

	if ((count != expectedcount) || (count<0)) RETURN_FALSE;
	for (i=0;i<count;i++)
	{
		 add_next_index_string(return_value,  names[i],1);
               free(names[i]);
	}

       efree(names);
}

PHP_FUNCTION(libvirt_storagepool_get_info)
{
         php_libvirt_storagepool *pool=NULL;
         zval *zpool;
         virStoragePoolInfo poolInfo;
         int retval;

         GET_STORAGEPOOL_FROM_ARGS("r",&zpool);

         retval=virStoragePoolGetInfo(pool->pool,&poolInfo);
         if (retval != 0) RETURN_FALSE;

         array_init(return_value);

		 LONGLONG_INIT
		 add_assoc_long (return_value, "state", (long)poolInfo.state);
		 LONGLONG_ASSOC (return_value, "capacity", poolInfo.capacity);
		 LONGLONG_ASSOC (return_value, "allocation", poolInfo.allocation);
		 LONGLONG_ASSOC (return_value, "available", poolInfo.available);
}




PHP_FUNCTION(libvirt_storagevolume_lookup_by_name)
{
         //php_libvirt_connection *conn=NULL;
	 php_libvirt_storagepool *pool=NULL;
         php_libvirt_volume *res_volume;
         //zval *zconn;
         zval *zpool;
         //zval *zvolume;
         int name_len;
         char *name=NULL;
	 virStorageVolPtr volume=NULL;

         GET_STORAGEPOOL_FROM_ARGS("rs",&zpool,&name,&name_len);


         if ( (name == NULL) || (name_len<1)) RETURN_FALSE;
         volume=virStorageVolLookupByName (pool->pool,name);
         if (volume==NULL) RETURN_FALSE;

         res_volume = emalloc(sizeof(php_libvirt_volume));
         res_volume->volume = volume;

         ZEND_REGISTER_RESOURCE(return_value, res_volume, le_libvirt_volume);
}

PHP_FUNCTION(libvirt_storagevolume_get_info)
{
         php_libvirt_volume *volume=NULL;
         zval *zvolume;
         virStorageVolInfo volumeInfo;
         int retval;

         GET_VOLUME_FROM_ARGS("r",&zvolume);

         retval=virStorageVolGetInfo(volume->volume,&volumeInfo);
         if (retval != 0) RETURN_FALSE;

         array_init(return_value);
         LONGLONG_INIT
         add_assoc_long(return_value, "type", (long)volumeInfo.type);
         LONGLONG_ASSOC(return_value, "capacity", volumeInfo.capacity);
         LONGLONG_ASSOC(return_value, "allocation", volumeInfo.allocation);
}

PHP_FUNCTION(libvirt_storagevolume_get_xml_desc)
{
         php_libvirt_volume *volume=NULL;
         zval *zvolume;
         char *xml;
         char *xml_out;
         long flags=0;

         GET_VOLUME_FROM_ARGS("r|l",&zvolume,&flags);

         xml=virStorageVolGetXMLDesc(volume->volume,flags);
         if (xml==NULL) RETURN_FALSE;

         RECREATE_STRING_WITH_E(xml_out,xml);

         RETURN_STRING(xml_out,0);
}

PHP_FUNCTION(libvirt_storagevolume_create_xml)
{
         php_libvirt_volume *res_volume=NULL;
	 php_libvirt_storagepool *pool=NULL;
         zval *zpool;
         virStorageVolPtr volume=NULL;
         char *xml;
         int xml_len;

         GET_STORAGEPOOL_FROM_ARGS("rs",&zpool,&xml,&xml_len);

         volume=virStorageVolCreateXML(pool->pool,xml,0);

         if (volume==NULL) RETURN_FALSE;

         res_volume= emalloc(sizeof(php_libvirt_volume));
         res_volume->volume = volume;

         ZEND_REGISTER_RESOURCE(return_value, res_volume, le_libvirt_volume);
}

PHP_FUNCTION(libvirt_list_domains)
{
	 php_libvirt_connection *conn=NULL;
	 zval *zconn;
	 zval *zdomain;
	 int count=-1;
	 //int maxids=-1;
	 int expectedcount=-1;
	 int *ids;
	 char **names;
	 int i;
	 
	 virDomainPtr domain=NULL;
	 php_libvirt_domain *res_domain;

	GET_CONNECTION_FROM_ARGS("r",&zconn);
	  
	  expectedcount=virConnectNumOfDomains (conn->conn);
	  
	  ids=emalloc(sizeof(int)*expectedcount);
	  count=virConnectListDomains (conn->conn,ids,expectedcount);
	  if ((count != expectedcount) || (count<0)) RETURN_FALSE;
	  array_init(return_value);
	  for (i=0;i<count;i++)
	  {
	     domain=virDomainLookupByID	(conn->conn,ids[i]);
	      if (domain!=NULL) 
	      {
	 
		      res_domain= emalloc(sizeof(php_libvirt_domain));
		      res_domain->domain = domain;

		      ALLOC_INIT_ZVAL(zdomain);
		      res_domain->conn=conn;
		      
		      ZEND_REGISTER_RESOURCE(zdomain, res_domain, le_libvirt_domain);
		      add_next_index_zval(return_value,  zdomain);

	      }
	  }
  	  efree(ids);
	  
	  expectedcount=virConnectNumOfDefinedDomains (conn->conn);
	  names=emalloc(expectedcount*sizeof(char *));
	  count=virConnectListDefinedDomains (conn->conn,names	,expectedcount);
	  if ((count != expectedcount) || (count<0)) RETURN_FALSE;
	  for (i=0;i<count;i++)
	  {
		 domain=virDomainLookupByName	(conn->conn,names[i]);
		 if (domain!=NULL) 
		 {
	 
		      res_domain= emalloc(sizeof(php_libvirt_domain));
		      res_domain->domain = domain;

		      ALLOC_INIT_ZVAL(zdomain);
                      res_domain->conn=conn;
		      
		      ZEND_REGISTER_RESOURCE(zdomain, res_domain, le_libvirt_domain);
		      add_next_index_zval(return_value,  zdomain);

		 } 
		  free(names[i]);
	  }
	  efree(names);
	  
}

PHP_FUNCTION(libvirt_list_domain_names)
{
	php_libvirt_connection *conn=NULL;
	zval *zconn;
	//zval *zdomain;
	int count=-1;
	//int maxids=-1;
	int expectedcount=-1;
	int *ids;
	char **names;
	const char *domain_name;
	int i;

	virDomainPtr domain=NULL;

	GET_CONNECTION_FROM_ARGS("r",&zconn);

	expectedcount=virConnectNumOfDomains (conn->conn);

	ids=emalloc(sizeof(int)*expectedcount);
	count=virConnectListDomains (conn->conn,ids,expectedcount);
	if ((count != expectedcount) || (count<0)) RETURN_FALSE;
	array_init(return_value);
	for (i=0;i<count;i++)
	{
		domain=virDomainLookupByID	(conn->conn,ids[i]);
		if (domain!=NULL) 
		{
			domain_name =  virDomainGetName (domain);
			add_next_index_string (return_value, estrndup (domain_name, strlen (domain_name)), 1);
			virDomainFree (domain);
		}
	}
	efree(ids);

	expectedcount=virConnectNumOfDefinedDomains (conn->conn);
	names=emalloc(expectedcount*sizeof(char *));
	count=virConnectListDefinedDomains (conn->conn,names	,expectedcount);
	if ((count != expectedcount) || (count<0)) RETURN_FALSE;
	for (i=0;i<count;i++)
	{
		domain=virDomainLookupByName	(conn->conn,names[i]);
		if (domain!=NULL) 
		{
			domain_name =  virDomainGetName (domain);
			add_next_index_string (return_value, estrndup (domain_name, strlen (domain_name)), 1);
			virDomainFree (domain);
		} 
		free(names[i]);
	}
	efree(names);
}

PHP_FUNCTION(libvirt_list_active_domains)
{
	 php_libvirt_connection *conn=NULL;
	 zval *zconn;
	 //zval *zdomain;
	 int count=-1;
	 //int maxids=-1;
	 int expectedcount=-1;
	 int *ids;
	 //char **names;
	 int i;
	 
	// virDomainPtr domain=NULL;
	 //php_libvirt_domain *res_domain;

	GET_CONNECTION_FROM_ARGS("r",&zconn);
	  
	  expectedcount=virConnectNumOfDomains (conn->conn);
	  
	  ids=emalloc(sizeof(int)*expectedcount);
	  count=virConnectListDomains (conn->conn,ids,expectedcount);
	  if ((count != expectedcount) || (count<0)) RETURN_FALSE;
	  array_init(return_value);
	  for (i=0;i<count;i++)
	  {
		  add_next_index_long(return_value,  ids[i]);
	  }
  	  efree(ids);
	  
	  
	  
}

PHP_FUNCTION(libvirt_list_defined_domains)
{
	 php_libvirt_connection *conn=NULL;
	 zval *zconn;
	 //zval *zdomain;
	 int count=-1;
	 //int maxids=-1;
	 int expectedcount=-1;
	 //int *ids;
	 char **names;
	 int i;
	 
	 //virDomainPtr domain=NULL;
	 //php_libvirt_domain *res_domain;

	GET_CONNECTION_FROM_ARGS("r",&zconn);
	  

	  array_init(return_value);
	  expectedcount=virConnectNumOfDefinedDomains (conn->conn);

	  names=emalloc(expectedcount*sizeof(char *));
	  count=virConnectListDefinedDomains (conn->conn,names	,expectedcount);
	  if ((count != expectedcount) || (count<0)) RETURN_FALSE;
	  for (i=0;i<count;i++)
	  {
		 add_next_index_string(return_value,  names[i],1);
		 free(names[i]);
	  }
	  efree(names);
	  
}

PHP_FUNCTION(libvirt_domain_get_name)
{

	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 const char *name=NULL;
	 //char *name_out;

	GET_DOMAIN_FROM_ARGS("r",&zdomain);
	
	 name=virDomainGetName(domain->domain);
	 if (name==NULL) RETURN_FALSE;
	  
	  RETURN_STRING(name,1);  //we can use the copy mechanism as we need not to free name (we even can not!)
}

PHP_FUNCTION(libvirt_domain_get_uuid_string)
{

	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 char *uuid;
	 //char *uuid_out;
	 int retval;

	GET_DOMAIN_FROM_ARGS("r",&zdomain);
	 
	 uuid=emalloc(VIR_UUID_STRING_BUFLEN);
	 retval=virDomainGetUUIDString(domain->domain,uuid);
	 if (retval!=0) RETURN_FALSE;
	  
	 RETURN_STRING(uuid,0);
}


PHP_FUNCTION(libvirt_domain_get_uuid)
{

	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 char *uuid;
	 //char *uuid_out;
	 int retval;

	GET_DOMAIN_FROM_ARGS("r",&zdomain);
	 
	 uuid=emalloc(VIR_UUID_BUFLEN);
	 retval=virDomainGetUUID(domain->domain,(unsigned char *)uuid);
	 if (retval!=0) RETURN_FALSE;
	  
	  RETURN_STRING(uuid,0);
}


PHP_FUNCTION(libvirt_domain_get_id)
{

	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 int retval;

	GET_DOMAIN_FROM_ARGS("r",&zdomain);
	 
	 retval=virDomainGetID(domain->domain);
	 	  
	 RETURN_LONG(retval);
}

PHP_FUNCTION(libvirt_domain_get_xml_desc)
{

	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 char *xml;
	 char *xml_out;
	 long flags=0;

	 GET_DOMAIN_FROM_ARGS("r|l",&zdomain,&flags);
	 
	 xml=virDomainGetXMLDesc(domain->domain,flags);
	 if (xml==NULL) RETURN_FALSE;
	  
	 RECREATE_STRING_WITH_E(xml_out,xml);
	 
	 RETURN_STRING(xml_out,0);
}


PHP_FUNCTION(libvirt_domain_get_info)
{

	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 virDomainInfo domainInfo;
	 int retval;
	 
	 GET_DOMAIN_FROM_ARGS("r",&zdomain);
	 
	 retval=virDomainGetInfo(domain->domain,&domainInfo);
	 if (retval != 0) RETURN_FALSE;
	  
	 array_init(return_value);
	 add_assoc_long(return_value, "maxMem", domainInfo.maxMem);
	 add_assoc_long(return_value, "memory", domainInfo.memory);
	 add_assoc_long(return_value, "state", (long)domainInfo.state);
	 add_assoc_long(return_value, "nrVirtCpu", domainInfo.nrVirtCpu);
	 add_assoc_double(return_value, "cpuUsed", (double)((double)domainInfo.cpuTime/1000000000.0));

}



PHP_FUNCTION(libvirt_domain_create)
{
	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 int retval;
	 
	 GET_DOMAIN_FROM_ARGS("r",&zdomain);
	 
	 retval=virDomainCreate(domain->domain);
	 if (retval != 0) RETURN_FALSE;
	 RETURN_TRUE;
}

PHP_FUNCTION(libvirt_domain_destroy)
{
	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 int retval;
	 
	 GET_DOMAIN_FROM_ARGS("r",&zdomain);
	 
	 retval=virDomainDestroy(domain->domain);
	 if (retval != 0) RETURN_FALSE;
	 RETURN_TRUE;
}

PHP_FUNCTION(libvirt_domain_resume)
{
	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 int retval;
	 
	 GET_DOMAIN_FROM_ARGS("r",&zdomain);
	 
	 retval=virDomainResume(domain->domain);
	 if (retval != 0) RETURN_FALSE;
	 RETURN_TRUE;
}

PHP_FUNCTION(libvirt_domain_shutdown)
{
	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 int retval;
	 
	 GET_DOMAIN_FROM_ARGS("r",&zdomain);
	 
	 retval=virDomainShutdown(domain->domain);
	 if (retval != 0) RETURN_FALSE;
	 RETURN_TRUE;
}


PHP_FUNCTION(libvirt_domain_suspend)
{
	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 int retval;
	 
	 GET_DOMAIN_FROM_ARGS("r",&zdomain);
	 
	 retval=virDomainSuspend(domain->domain);
	 if (retval != 0) RETURN_FALSE;
	 RETURN_TRUE;
}

PHP_FUNCTION(libvirt_domain_undefine)
{
	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 int retval;
	 
	 GET_DOMAIN_FROM_ARGS("r",&zdomain);
	 
	 retval=virDomainUndefine(domain->domain);
	 if (retval != 0) RETURN_FALSE;
	 RETURN_TRUE;
}

PHP_FUNCTION(libvirt_domain_reboot)
{
	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 int retval;
	 long flags=0;
	 
	 GET_DOMAIN_FROM_ARGS("r|l",&zdomain,&flags);
	 
	 retval=virDomainReboot(domain->domain,flags);
	 if (retval != 0) RETURN_FALSE;
	 RETURN_TRUE;
}

PHP_FUNCTION(libvirt_domain_define_xml)
{

php_libvirt_domain *res_domain=NULL;
	 php_libvirt_connection *conn=NULL;
	 zval *zconn;
	 virDomainPtr domain=NULL;
	 char *xml;
	 int xml_len;
	 
	 GET_CONNECTION_FROM_ARGS("rs",&zconn,&xml,&xml_len);
	 
	 domain=virDomainDefineXML(conn->conn,xml);
	 
	 if (domain==NULL) RETURN_FALSE;
	 
	 res_domain= emalloc(sizeof(php_libvirt_domain));
	 res_domain->domain = domain;
     
         res_domain->conn=conn;

	ZEND_REGISTER_RESOURCE(return_value, res_domain, le_libvirt_domain);
}

PHP_FUNCTION(libvirt_domain_create_xml)
{

	 php_libvirt_domain *res_domain=NULL;
	 php_libvirt_connection *conn=NULL;
	 zval *zconn;
	 virDomainPtr domain=NULL;
	 char *xml;
	 int xml_len;
	 
	 GET_CONNECTION_FROM_ARGS("rs",&zconn,&xml,&xml_len);
	 
	 domain=virDomainCreateXML(conn->conn,xml,0);
	 
	 if (domain==NULL) RETURN_FALSE;
	 
	 res_domain= emalloc(sizeof(php_libvirt_domain));
	 res_domain->domain = domain;

	 res_domain->conn=conn;

	ZEND_REGISTER_RESOURCE(return_value, res_domain, le_libvirt_domain);
}



PHP_FUNCTION(libvirt_domain_memory_peek)
{
	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 int retval;
	 long flags=0;
	 long long start;
	 long size;
	 char *buff;
	 
	 GET_DOMAIN_FROM_ARGS("rlll",&zdomain,&start,&size,&flags);
	 buff=emalloc(size);
	 retval=virDomainMemoryPeek(domain->domain,start,size,buff,flags);
	 
	 if (retval != 0) RETURN_FALSE;
	 RETURN_STRINGL(buff,size,0);
}

//0.7.5+
#if LIBVIR_VERSION_NUMBER>=7005
PHP_FUNCTION(libvirt_domain_memory_stats)
{
	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 int retval;
	 long flags=0;
	 int i;
	 struct _virDomainMemoryStat stats[VIR_DOMAIN_MEMORY_STAT_NR];
	 	 
	 GET_DOMAIN_FROM_ARGS("r|l",&zdomain,&flags);
	 
	 retval=virDomainMemoryStats(domain->domain,stats,VIR_DOMAIN_MEMORY_STAT_NR,flags);
	 
	 if (retval == -1) RETURN_FALSE;
	 LONGLONG_INIT
	 array_init(return_value);
	 for (i=0;i<retval;i++)
	 {
		 LONGLONG_INDEX(return_value, stats[i].tag,stats[i].val)
	 }
	 
	 
}
#endif

PHP_FUNCTION(libvirt_domain_block_stats)
{
	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 int retval;
	 //long flags=0;
	 //int i;
	 char *path;
	 int path_len;
	 	 	 
	 struct _virDomainBlockStats stats;
	  
	 GET_DOMAIN_FROM_ARGS("rs",&zdomain,&path,&path_len);
	 
	 retval=virDomainBlockStats(domain->domain,path,&stats, sizeof stats);
	 
	 if (retval == -1) RETURN_FALSE;
	 
	 array_init(return_value);
	 LONGLONG_INIT
	 LONGLONG_ASSOC(return_value, "rd_req", stats.rd_req)
	 LONGLONG_ASSOC(return_value, "rd_bytes", stats.rd_bytes);
	 LONGLONG_ASSOC(return_value, "wr_req", stats.wr_req);
	 LONGLONG_ASSOC(return_value, "wr_bytes", stats.wr_bytes);
	 LONGLONG_ASSOC(return_value, "errs", stats.errs);
}

PHP_FUNCTION(libvirt_domain_interface_stats)
{
	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 int retval;
	 //long flags=0;
	 //int i;
	 char *path;
	 int path_len;
	 	 	 
	 struct _virDomainInterfaceStats stats;
	  
	 GET_DOMAIN_FROM_ARGS("rs",&zdomain,&path,&path_len);
	 
	 retval=virDomainInterfaceStats(domain->domain,path,&stats, sizeof stats);
	 
	 if (retval == -1) RETURN_FALSE;
	 
	 array_init(return_value);
	 LONGLONG_INIT
	 LONGLONG_ASSOC(return_value, "rx_bytes", stats.rx_bytes);
	 LONGLONG_ASSOC(return_value, "rx_packets", stats.rx_packets);
	 LONGLONG_ASSOC(return_value, "rx_errs", stats.rx_errs);
	 LONGLONG_ASSOC(return_value, "rx_drop", stats.rx_drop);
	 LONGLONG_ASSOC(return_value, "tx_bytes", stats.tx_bytes);
	 LONGLONG_ASSOC(return_value, "tx_packets", stats.tx_packets);
	 LONGLONG_ASSOC(return_value, "tx_errs", stats.tx_errs);
	 LONGLONG_ASSOC(return_value, "tx_drop", stats.tx_drop);
	 
}

PHP_FUNCTION(libvirt_version)
{
	unsigned long libVer;
	unsigned long typeVer;
	int type_len;
	char *type=NULL;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &type,&type_len) == FAILURE) {
		RETURN_FALSE;
	}
	
	if (virGetVersion	(&libVer,type,&typeVer)!= 0)
		RETURN_FALSE;
	
	//major * 1,000,000 + minor * 1,000 + release.
	array_init(return_value);
	
	add_assoc_long(return_value, "libvirt.release",(long)(libVer %1000));
	add_assoc_long(return_value, "libvirt.minor",(long)((libVer/1000) % 1000));
	add_assoc_long(return_value, "libvirt.major",(long)((libVer/1000000) % 1000));
	
	add_assoc_long(return_value, "type.release",(long)(typeVer %1000));
	add_assoc_long(return_value, "type.minor",(long)((typeVer/1000) % 1000));
	add_assoc_long(return_value, "type.major",(long)((typeVer/1000000) % 1000));
					 
	
}


PHP_FUNCTION(libvirt_domain_get_connect)
{

	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
         php_libvirt_connection *conn;

	GET_DOMAIN_FROM_ARGS("r",&zdomain);

	 conn= domain->conn;
	 //virConnectRef (conn->conn);
	 if (conn->conn == NULL) RETURN_FALSE;
         RETURN_RESOURCE(conn->resource_id);
}



PHP_FUNCTION(libvirt_domain_migrate_to_uri)
{
	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 int retval;
	 long flags=0;
	 //int i;
	 char *duri;
	 int duri_len;
	 char *dname;
	 int dname_len;
         long bandwith;	 
	 	 	 
	 
	 dname=NULL;
	 dname_len=0;
	 bandwith=0;
	 GET_DOMAIN_FROM_ARGS("rsl|sl",&zdomain,&duri,&duri_len,&flags,&dname,&dname_len,&bandwith);
	 
	 retval=virDomainMigrateToURI(domain->domain,duri,flags,dname,bandwith);
	 
	 if (retval == 0) RETURN_TRUE;
	 RETURN_FALSE;
	 

	 
}


PHP_FUNCTION(libvirt_domain_migrate)
{
	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 
	 php_libvirt_connection *dconn=NULL;
	 zval *zdconn;
	 
	 virDomainPtr destdomain=NULL;
	 php_libvirt_domain *res_domain;
	 
	 //int retval;
	 long flags=0;
	 //int i;
	 char *dname;
	 int dname_len;
         long bandwith;
         char *uri;
         int uri_len;	 
	 	 	 
	 
	 dname=NULL;
	 dname_len=0;
	 bandwith=0;
	 uri_len=0;
	 uri=NULL;
	 GET_DOMAIN_FROM_ARGS("rrl|sl",&zdomain,&zdconn,&flags,&dname,&dname_len,&uri,&uri_len,&bandwith);
	 ZEND_FETCH_RESOURCE(dconn, php_libvirt_connection*, &zdconn, -1, PHP_LIBVIRT_CONNECTION_RES_NAME, le_libvirt_connection);
	  if ((dconn==NULL) || (dconn->conn==NULL)) RETURN_FALSE;
	 
	 destdomain=virDomainMigrate(domain->domain,dconn->conn,flags,dname,uri,bandwith);
	 
	 if (destdomain == NULL) RETURN_FALSE;
	 
	 res_domain= emalloc(sizeof(php_libvirt_domain));
	 res_domain->domain = destdomain;
         res_domain->conn=dconn;

 	 ZEND_REGISTER_RESOURCE(return_value, res_domain, le_libvirt_domain);
	 	 
}

  

#if LIBVIR_VERSION_NUMBER>=7007
PHP_FUNCTION(libvirt_domain_get_job_info)
{
	 php_libvirt_domain *domain=NULL;
	 zval *zdomain;
	 int retval;
	 	 	 	 
	 struct _virDomainJobInfo jobinfo;
	  
	 GET_DOMAIN_FROM_ARGS("r",&zdomain);
	 
	 retval=virDomainGetJobInfo(domain->domain,&jobinfo);
	 
	 if (retval == -1) RETURN_FALSE;
	 
	 array_init(return_value);
         LONGLONG_INIT
         add_assoc_long(return_value, "type", jobinfo.type);	 
	 LONGLONG_ASSOC(return_value, "time_elapsed", jobinfo.timeElapsed);
	 LONGLONG_ASSOC(return_value, "time_remaining", jobinfo.timeRemaining);
	 LONGLONG_ASSOC(return_value, "data_total", jobinfo.dataTotal);
	 LONGLONG_ASSOC(return_value, "data_processed", jobinfo.dataProcessed);
	 LONGLONG_ASSOC(return_value, "data_remaining", jobinfo.dataRemaining);
	 LONGLONG_ASSOC(return_value, "mem_total", jobinfo.memTotal);
	 LONGLONG_ASSOC(return_value, "mem_processed", jobinfo.memProcessed);
	 LONGLONG_ASSOC(return_value, "mem_remaining", jobinfo.memRemaining);
	 LONGLONG_ASSOC(return_value, "file_total", jobinfo.fileTotal);
	 LONGLONG_ASSOC(return_value, "file_processed", jobinfo.fileProcessed);
	 LONGLONG_ASSOC(return_value, "file_remaining", jobinfo.fileRemaining);
	 
}
#endif


PHP_FUNCTION(libvirt_storagepool_get_name)
{
	php_libvirt_storagepool *pool = NULL;
	zval *zpool;
	const char *name=NULL;

	GET_STORAGEPOOL_FROM_ARGS("r", &zpool);

	name = virStoragePoolGetName (pool->pool);
	if (name == NULL)
	{
		RETURN_FALSE;
	}

	RETURN_STRING(name, 1);
}

PHP_FUNCTION(libvirt_storagepool_get_uuid_string)
{
	php_libvirt_storagepool *pool=NULL;
	zval *zpool;
	char *uuid;

	GET_STORAGEPOOL_FROM_ARGS ("r", &zpool);

	uuid = emalloc (VIR_UUID_STRING_BUFLEN);
	if (virStoragePoolGetUUIDString (pool->pool, uuid) != 0)
	{
		RETURN_FALSE;
	}

	RETURN_STRING(uuid, 0);
}

PHP_FUNCTION(libvirt_storagepool_lookup_by_uuid_string)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	char *uuid = NULL;
	int uuid_len;
	virStoragePoolPtr storage=NULL;
	php_libvirt_storagepool *res_pool;

	GET_CONNECTION_FROM_ARGS("rs", &zconn, &uuid, &uuid_len);

	if ((uuid == NULL) || (uuid_len < 1))
	{
		RETURN_FALSE;
	}
	storage = virStoragePoolLookupByUUIDString (conn->conn, uuid);
	if (storage == NULL)
	{
		RETURN_FALSE;
	}

	res_pool = emalloc (sizeof (php_libvirt_storagepool));
	res_pool->pool = storage;
	res_pool->conn = conn;

	ZEND_REGISTER_RESOURCE (return_value, res_pool, le_libvirt_storagepool);
}


PHP_FUNCTION (libvirt_list_storagepools)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	int count;
	int i;
	
	GET_CONNECTION_FROM_ARGS("r", &zconn);

	count = virConnectNumOfStoragePools (conn->conn);
	char *names[count];
	count = virConnectListStoragePools (conn->conn, names, count);

	if (count < 0)
	{
		RETURN_FALSE;
	}

	array_init (return_value);
	for(i = 0; i < count; i++)
	{
		add_next_index_string (return_value, names[i], 1);
		free (names[i]);
	}

	count = virConnectNumOfDefinedStoragePools (conn->conn);
	if (count < 0)
	{
		RETURN_FALSE;
	}
	char *namez[count];
	count = virConnectListDefinedStoragePools (conn->conn, namez, count);

	for (i = 0; i < count; i++)
	{
		add_next_index_string (return_value, namez[i], 1);
		free (namez[i]);
	}
}

PHP_FUNCTION (libvirt_get_domain_state_string)
{
	static char *state_strings[] = 
	{
		"VIR_DOMAIN_NOSTATE",
		"VIR_DOMAIN_RUNNING",
		"VIR_DOMAIN_BLOCKED",
		"VIR_DOMAIN_PAUSED",
		"VIR_DOMAIN_SHUTDOWN",
		"VIR_DOMAIN_SHUTOFF",
		"VIR_DOAMIN_CRASHED"
	};
	int state;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "l", &state) == FAILURE)
	{
		RETURN_FALSE;
	}

	if (state < 0 || state > ARRAY_SIZE (state_strings))
	{
		RETURN_STRING ("Err: Unknown State", 1);
	}

	RETURN_STRING (state_strings[state], 1);
}

PHP_FUNCTION (libvirt_get_storagepool_state_string)
{
	static char *state_strings[] = 
	{
		"VIR_STORAGE_POOL_INACTIVE",
		"VIR_STORAGE_POOL_BUILDING",
		"VIR_STORAGE_POOL_RUNNING",
		"VIR_STORAGE_POOL_DEGRADED",
		"VIR_STORAGE_POOL_INACCESSIBLE"
	};
	int state;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "l", &state) == FAILURE)
	{
		RETURN_FALSE;
	}

	if (state < 0 || state > ARRAY_SIZE (state_strings))
	{
		RETURN_STRING ("Err: Unknown State", 1);
	}

	RETURN_STRING (state_strings[state], 1);
}

PHP_FUNCTION(libvirt_storagepool_get_xml_desc)
{
	php_libvirt_storagepool *pool = NULL;
	zval *zpool;
	char *xml;
	char *xml_out;
	long flags = 0;

	GET_STORAGEPOOL_FROM_ARGS("r|l", &zpool, &flags);

	xml = virStoragePoolGetXMLDesc (pool->pool, flags);
	if (xml == NULL)
	{
		RETURN_FALSE;
	}

	RECREATE_STRING_WITH_E (xml_out, xml);
	RETURN_STRING (xml_out, 1);
}

PHP_FUNCTION(libvirt_get_storagepool_count)
{
	 php_libvirt_connection *conn=NULL;
	 zval *zconn;
	 int a, i;

	 GET_CONNECTION_FROM_ARGS("r",&zconn);
	  
	 a=virConnectNumOfStoragePools (conn->conn);
	 if (a < 0) RETURN_FALSE;
	 i=virConnectNumOfDefinedStoragePools (conn->conn);
	 if (i < 0) RETURN_FALSE;

	 RETURN_LONG(a + i);
}

PHP_FUNCTION(libvirt_get_active_storagepool_count)
{
	 php_libvirt_connection *conn=NULL;
	 zval *zconn;
	 int count;

	 GET_CONNECTION_FROM_ARGS("r",&zconn);
	  
	 count=virConnectNumOfStoragePools (conn->conn);
	 if (count <0) RETURN_FALSE;
	 RETURN_LONG(count);
}

PHP_FUNCTION(libvirt_get_defined_storagepool_count)
{
	 php_libvirt_connection *conn=NULL;
	 zval *zconn;
	 int count;

	 GET_CONNECTION_FROM_ARGS("r",&zconn);
	  
	 count=virConnectNumOfDefinedStoragePools (conn->conn);
	 if (count <0) RETURN_FALSE;
	 RETURN_LONG(count);
}

PHP_FUNCTION(libvirt_get_capabilities)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	char *xml;
	char *xml_out;

	GET_CONNECTION_FROM_ARGS("r", &zconn);

	xml = virConnectGetCapabilities (conn->conn);
	if (xml == NULL)
	{
		RETURN_FALSE;
	}

	RECREATE_STRING_WITH_E (xml_out, xml);
	RETURN_STRING (xml_out, 0);
}

PHP_FUNCTION(libvirt_get_max_vcpus)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	int val;
	char *type;
	int type_len;

	GET_CONNECTION_FROM_ARGS ("rs", &zconn, &type, &type_len);

	val = virConnectGetMaxVcpus (conn->conn, type);
	if (val < 0)
	{
		RETURN_FALSE;
	}

	RETURN_LONG (val);
}

PHP_FUNCTION(libvirt_node_get_free_memory)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	int val;

	GET_CONNECTION_FROM_ARGS ("r", &zconn);

	val = virNodeGetFreeMemory (conn->conn);
	if (val < 0)
	{
		RETURN_FALSE;
	}

	RETURN_LONGLONG (val);
}

PHP_FUNCTION(libvirt_get_type)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	const char *val;

	GET_CONNECTION_FROM_ARGS ("r", &zconn);

	val = virConnectGetType (conn->conn);
	if (val == NULL)
	{
		RETURN_FALSE;
	}

	RETURN_STRING (estrndup (val, strlen(val)), 0);
}

PHP_FUNCTION(libvirt_get_version)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	unsigned long val;

	GET_CONNECTION_FROM_ARGS ("r", &zconn);

	val = virConnectGetVersion (conn->conn, &val);
	if (val < 0)
	{
		RETURN_FALSE;
	}

	RETURN_LONGLONG (val);
}

PHP_FUNCTION(libvirt_get_lib_version)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	unsigned long val;

	GET_CONNECTION_FROM_ARGS ("r", &zconn);

	if (virConnectGetLibVersion (conn->conn, &val) < 0)
	{
		RETURN_FALSE;
	}

	RETURN_LONGLONG (val);
}

PHP_FUNCTION(libvirt_get_uri)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	char *val;
	char *val_out;

	GET_CONNECTION_FROM_ARGS ("r", &zconn);

	val = virConnectGetURI (conn->conn);
	if (val == NULL)
	{
		RETURN_FALSE;
	}

	RECREATE_STRING_WITH_E (val_out, val);
	RETURN_STRING (val_out, 0);
}

PHP_FUNCTION(libvirt_storagepool_define_xml)
{
	php_libvirt_storagepool *res_pool = NULL;
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	virStoragePoolPtr pool = NULL;
	char *xml;
	int xml_len;
	long flags = 0;


	GET_CONNECTION_FROM_ARGS ("rs|l", &zconn, &xml, &xml_len, &flags);

	pool = virStoragePoolDefineXML (conn->conn, xml, (unsigned int)flags);
	if (pool == NULL)
	{
		RETURN_FALSE;
	}

	res_pool = emalloc (sizeof (php_libvirt_storagepool));
	res_pool->pool = pool;
	res_pool->conn = conn;

	ZEND_REGISTER_RESOURCE (return_value, res_pool, le_libvirt_storagepool);
}

PHP_FUNCTION(libvirt_storagepool_undefine)
{
	php_libvirt_storagepool *pool = NULL;
	zval *zpool;
	
	GET_STORAGEPOOL_FROM_ARGS ("r", &zpool);

	if (virStoragePoolUndefine (pool->pool) != 0)
	{
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_storagepool_create)
{
	php_libvirt_storagepool *pool = NULL;
	zval *zpool;

	GET_STORAGEPOOL_FROM_ARGS ("r", &zpool);

	if (virStoragePoolCreate (pool->pool, 0) != 0)
	{
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_storagepool_destroy)
{
	php_libvirt_storagepool *pool = NULL;
	zval *zpool;

	GET_STORAGEPOOL_FROM_ARGS ("r", &zpool);

	if (virStoragePoolDestroy (pool->pool) != 0)
	{
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_storagepool_get_connect)
{
	php_libvirt_connection *conn = NULL;
	php_libvirt_storagepool *pool = NULL;
	zval *zpool;

	GET_STORAGEPOOL_FROM_ARGS ("r", &zpool);

	conn = pool->conn;
	if (conn->conn == NULL)
	{
		RETURN_FALSE;
	}

	RETURN_RESOURCE (conn->resource_id);
}

PHP_FUNCTION(libvirt_storagepool_is_active)
{
	php_libvirt_storagepool *pool = NULL;
	zval *zpool;

	GET_STORAGEPOOL_FROM_ARGS ("r", &zpool);
	
	RETURN_LONG (virStoragePoolIsActive (pool->pool));
}

PHP_FUNCTION(libvirt_storagepool_get_volume_count)
{
	php_libvirt_storagepool *pool = NULL;
	zval *zpool;

	GET_STORAGEPOOL_FROM_ARGS ("r", &zpool);

	RETURN_LONG (virStoragePoolNumOfVolumes(pool->pool));
}

PHP_FUNCTION(libvirt_storagepool_refresh)
{
	php_libvirt_storagepool *pool = NULL;
	zval *zpool;

	GET_STORAGEPOOL_FROM_ARGS ("r", &zpool);

	if (virStoragePoolRefresh (pool->pool, 0) < 0)
	{
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_storagepool_set_autostart)
{
	php_libvirt_storagepool *pool = NULL;
	zval *zpool;
	long flags = 0;

	GET_STORAGEPOOL_FROM_ARGS ("rl", &zpool, &flags);

	if (virStoragePoolSetAutostart (pool->pool, flags) != 0)
	{
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_storagepool_get_autostart)
{
	php_libvirt_storagepool *pool = NULL;
	zval *zpool;
	int flags = 0;

	GET_STORAGEPOOL_FROM_ARGS ("r", &zpool);

	if (virStoragePoolGetAutostart (pool->pool, &flags) == 0)
	{
		RETURN_LONG ((long)flags);
	}
	else
	{
		RETURN_FALSE;
	}
}

PHP_FUNCTION(libvirt_storagevolume_get_path)
{
	php_libvirt_volume *volume = NULL;
	zval *zvolume;
	char *val;

	GET_VOLUME_FROM_ARGS ("r", &zvolume);

	val = virStorageVolGetPath (volume->volume);
	if (val == NULL)
	{
		RETURN_FALSE;
	}

	RETURN_STRING (val, 1);
}

PHP_FUNCTION(libvirt_storagevolume_get_name)
{
	php_libvirt_volume *volume = NULL;
	zval *zvolume;
	const char *val;

	GET_VOLUME_FROM_ARGS ("r", &zvolume);

	val = virStorageVolGetName (volume->volume);
	if (val == NULL)
	{
		RETURN_FALSE;
	}

	RETURN_STRING (val, 1);
}

PHP_FUNCTION (libvirt_get_storagevolume_type_string)
{
	static char *type_strings[] = 
	{
		"VIR_STORAGE_VOL_FILE",
		"VIR_STORAGE_VOL_BLOCK",
	};
	int state;

	if (zend_parse_parameters (ZEND_NUM_ARGS() TSRMLS_CC, "l", &state) == FAILURE)
	{
		RETURN_FALSE;
	}

	if (state < 0 || state > ARRAY_SIZE (type_strings))
	{
		RETURN_STRING ("Err: Unknown State", 1);
	}

	RETURN_STRING (type_strings[state], 1);
}

PHP_FUNCTION(libvirt_list_networks)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	int count;
	int i;

	GET_CONNECTION_FROM_ARGS("r", &zconn);

	count = virConnectNumOfNetworks (conn->conn);
	char *names[count];
	count = virConnectListNetworks (conn->conn, names, count);
	if (count < 0)
	{
		RETURN_FALSE;
	}

	array_init (return_value);
	for (i = 0; i < count; i++)
	{
		add_next_index_string (return_value, names[i], 1);
		free (names[i]);
	}

	count = virConnectNumOfDefinedNetworks (conn->conn);
	if (count < 0)
	{
		RETURN_FALSE;
	}
	char *namez[count];
	count = virConnectListDefinedNetworks (conn->conn, namez, count);
	for (i = 0; i < count; i++)
	{
		add_next_index_string (return_value, namez[i], 1);
		free (namez[i]);
	}
}

PHP_FUNCTION(libvirt_get_network_count)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	long val, val2;

	GET_CONNECTION_FROM_ARGS("r", &zconn);

	val = virConnectNumOfNetworks (conn->conn);
	if (val < 0)
	{
		RETURN_FALSE;
	}

	val2 = virConnectNumOfDefinedNetworks (conn->conn);
	if (val2 < 0)
	{
		RETURN_FALSE;
	}

	RETURN_LONG(val + val2);
}

PHP_FUNCTION(libvirt_get_active_network_count)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	long val;

	GET_CONNECTION_FROM_ARGS("r", &zconn);

	val = virConnectNumOfNetworks (conn->conn);
	if (val < 0)
	{
		RETURN_FALSE;
	}

	RETURN_LONG(val);
}

PHP_FUNCTION(libvirt_get_defined_network_count)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	long val;

	GET_CONNECTION_FROM_ARGS("r", &zconn);

	val = virConnectNumOfDefinedNetworks (conn->conn);
	if (val < 0)
	{
		RETURN_FALSE;
	}

	RETURN_LONG(val);
}

PHP_FUNCTION(libvirt_network_define_xml)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	php_libvirt_network *res_network;
	virNetworkPtr network;
	char *xml;
	int xml_len;

	GET_CONNECTION_FROM_ARGS ("r|s", &zconn, &xml, &xml_len);

	network = virNetworkDefineXML (conn->conn, xml);
	if (network == NULL)
	{		
		RETURN_FALSE;
	}

	res_network = emalloc (sizeof (php_libvirt_network));
	res_network->network = network;
	res_network->conn = conn;

	ZEND_REGISTER_RESOURCE (return_value, res_network, le_libvirt_network);
}

PHP_FUNCTION(libvirt_network_create)
{
	php_libvirt_network *network = NULL;
	zval *znetwork;
	
	GET_NETWORK_FROM_ARGS ("r", &znetwork);

	if (virNetworkCreate (network->network) != 0)
	{
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_network_destroy)
{
	php_libvirt_network *network = NULL;
	zval *znetwork;
	
	GET_NETWORK_FROM_ARGS ("r", &znetwork);

	if (virNetworkDestroy (network->network) < 0)
	{
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_network_undefine)
{
	php_libvirt_network *network = NULL;
	zval *znetwork;
	
	GET_NETWORK_FROM_ARGS ("r", &znetwork);

	if (virNetworkUndefine (network->network) < 0)
	{
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_network_get_autostart)
{
	php_libvirt_network *network = NULL;
	zval *znetwork;
	int rst;
	int flags = 0;
	
	GET_NETWORK_FROM_ARGS ("r", &znetwork);

	rst = virNetworkGetAutostart (network->network, &flags);
	if (rst != 0)
	{
		RETURN_FALSE;
	}
	else
	{
		RETURN_LONG ((long)flags);
	}
}

PHP_FUNCTION(libvirt_network_get_bridge_name)
{
	php_libvirt_network *network = NULL;
	zval *znetwork;
	char *val;
	char *val_out;

	GET_NETWORK_FROM_ARGS ("r", &znetwork);

	val = virNetworkGetBridgeName (network->network);
	if (val == NULL)
	{
		RETURN_FALSE;
	}
	
	RECREATE_STRING_WITH_E (val_out, val);

	RETURN_STRING (val_out, 0);
}

PHP_FUNCTION(libvirt_network_get_connect)
{
	php_libvirt_network *network = NULL;
	zval *znetwork;
	php_libvirt_connection *conn;

	GET_NETWORK_FROM_ARGS ("r", &znetwork);
	
	conn = network->conn;
	if (conn == NULL)
	{
		RETURN_FALSE;
	}
	
	RETURN_RESOURCE (conn->resource_id);
}

PHP_FUNCTION(libvirt_network_get_name)
{
	php_libvirt_network *network = NULL;
	zval *znetwork;
	const char *val;

	GET_NETWORK_FROM_ARGS ("r", &znetwork);

	val = virNetworkGetName (network->network);
	if (val == NULL)
	{
		RETURN_FALSE;
	}
	
	RETURN_STRING (val, 1);
}

PHP_FUNCTION(libvirt_network_get_uuid_string)
{
	php_libvirt_network *network = NULL;
	zval *znetwork;
	char *uuid;

	GET_NETWORK_FROM_ARGS ("r", &znetwork);

	uuid = emalloc (VIR_UUID_STRING_BUFLEN);
	if (virNetworkGetUUIDString (network->network, uuid) < 0)
	{
		RETURN_FALSE;
	}

	RETURN_STRING (uuid, 0);
}

PHP_FUNCTION(libvirt_network_get_xml_desc)
{
	php_libvirt_network *network = NULL;
	zval *znetwork;
	char *xml;
	char *xml_out;
	long flags = 0;

	GET_NETWORK_FROM_ARGS ("r|l", &znetwork, &flags);

	xml = virNetworkGetXMLDesc (network->network, (int)flags);
	if (xml == NULL)
	{
		RETURN_FALSE;
	}

	RECREATE_STRING_WITH_E (xml_out, xml);
	RETURN_STRING (xml_out, 0);
}

PHP_FUNCTION(libvirt_network_is_active)
{
	php_libvirt_network *network = NULL;
	zval *znetwork;

	GET_NETWORK_FROM_ARGS ("r", &znetwork);

	RETURN_LONG (virNetworkIsActive (network->network));
}

PHP_FUNCTION(libvirt_network_lookup_by_name)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	php_libvirt_network *res_network;
	virNetworkPtr network;
	char *name;
	int name_len;

	GET_CONNECTION_FROM_ARGS ("rs", &zconn, &name, &name_len);

	if ((name == NULL) || (name_len < 1))
	{
		RETURN_FALSE;
	}
	
	network = virNetworkLookupByName (conn->conn, name);
	if (network == NULL)
	{
		RETURN_FALSE;
	}

	res_network = emalloc (sizeof (php_libvirt_network));
	res_network->network = network;
	res_network->conn = conn;

	ZEND_REGISTER_RESOURCE (return_value, res_network, le_libvirt_network);
}

PHP_FUNCTION(libvirt_network_lookup_by_uuid_string)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	php_libvirt_network *res_network;
	virNetworkPtr network;
	char *name;
	int name_len;

	GET_CONNECTION_FROM_ARGS ("rs", &zconn, &name, &name_len);

	if ((name == NULL) || (name_len < 1))
	{
		RETURN_FALSE;
	}
	
	network = virNetworkLookupByUUIDString (conn->conn, name);
	if (network == NULL)
	{
		RETURN_FALSE;
	}

	res_network = emalloc (sizeof (php_libvirt_network));
	res_network->network = network;
	res_network->conn = conn;

	ZEND_REGISTER_RESOURCE (return_value, res_network, le_libvirt_network);
}

PHP_FUNCTION(libvirt_network_set_autostart)
{
	php_libvirt_network *network = NULL;
	zval *znetwork;
	int flags = 0;

	GET_NETWORK_FROM_ARGS ("rl", &znetwork, &flags);

	if (virNetworkSetAutostart (network->network, (long)flags) != 0)
	{
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_domain_get_autostart)
{
	php_libvirt_domain *domain = NULL;
	zval *zdomain;
	int flags = 0;

	GET_DOMAIN_FROM_ARGS ("r", &zdomain);

	if (virDomainGetAutostart (domain->domain, &flags) != 0)
	{
		RETURN_FALSE;
	}
	RETURN_LONG ((long)flags);
}

PHP_FUNCTION(libvirt_domain_set_autostart)
{
	php_libvirt_domain *domain = NULL;
	zval *zdomain;
	int flags = 0;

	GET_DOMAIN_FROM_ARGS ("rl", &zdomain, &flags);

	if (virDomainSetAutostart (domain->domain, flags) != 0)
	{
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_domain_is_active)
{
	php_libvirt_domain *domain = NULL;
	zval *zdomain;

	GET_DOMAIN_FROM_ARGS ("r", &zdomain);

	RETURN_LONG (virDomainIsActive(domain->domain));
}

PHP_FUNCTION(libvirt_list_active_domain_names)
{
		php_libvirt_connection *conn=NULL;
	zval *zconn;
	//zval *zdomain;
	int count=-1;
	//int maxids=-1;
	int expectedcount=-1;
	int *ids;
	const char *domain_name;
	int i;

	virDomainPtr domain=NULL;

	GET_CONNECTION_FROM_ARGS("r",&zconn);
	array_init(return_value);

	expectedcount=virConnectNumOfDomains (conn->conn);

	ids=emalloc(sizeof(int)*expectedcount);
	count=virConnectListDomains (conn->conn,ids,expectedcount);
	if ((count != expectedcount) || (count<0)) RETURN_FALSE;
	for (i=0;i<count;i++)
	{
		domain=virDomainLookupByID	(conn->conn,ids[i]);
		if (domain!=NULL) 
		{
			domain_name =  virDomainGetName (domain);
			add_next_index_string (return_value, estrndup (domain_name, strlen (domain_name)), 1);
			virDomainFree (domain);
		}
	}
	efree(ids);
}

PHP_FUNCTION(libvirt_list_defined_domain_names)
{
		php_libvirt_connection *conn=NULL;
	zval *zconn;
	//zval *zdomain;
	int count=-1;
	//int maxids=-1;
	int expectedcount=-1;
	char **names;
	const char *domain_name;
	int i;

	virDomainPtr domain=NULL;

	GET_CONNECTION_FROM_ARGS("r",&zconn);
	array_init(return_value);

	expectedcount=virConnectNumOfDomains (conn->conn);

	expectedcount=virConnectNumOfDefinedDomains (conn->conn);
	names=emalloc(expectedcount*sizeof(char *));
	count=virConnectListDefinedDomains (conn->conn,names	,expectedcount);
	if ((count != expectedcount) || (count<0)) RETURN_FALSE;
	for (i=0;i<count;i++)
	{
		domain=virDomainLookupByName	(conn->conn,names[i]);
		if (domain!=NULL) 
		{
			domain_name =  virDomainGetName (domain);
			add_next_index_string (return_value, estrndup (domain_name, strlen (domain_name)), 1);
			virDomainFree (domain);
		} 
		free(names[i]);
	}
	efree(names);
}

PHP_FUNCTION(libvirt_list_active_storagepools)
{
		php_libvirt_connection *conn = NULL;
	zval *zconn;
	int count;
	int i;
	
	GET_CONNECTION_FROM_ARGS("r", &zconn);
	array_init (return_value);

	count = virConnectNumOfStoragePools (conn->conn);
	char *names[count];
	count = virConnectListStoragePools (conn->conn, names, count);

	if (count < 0)
	{
		RETURN_FALSE;
	}

	for(i = 0; i < count; i++)
	{
		add_next_index_string (return_value, names[i], 1);
		free (names[i]);
	}
}

PHP_FUNCTION(libvirt_list_defined_storagepools)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	int count;
	int i;
	
	GET_CONNECTION_FROM_ARGS("r", &zconn);
	array_init (return_value);

	count = virConnectNumOfDefinedStoragePools (conn->conn);
	if (count < 0)
	{
		RETURN_FALSE;
	}
	char *namez[count];
	count = virConnectListDefinedStoragePools (conn->conn, namez, count);

	for (i = 0; i < count; i++)
	{
		add_next_index_string (return_value, namez[i], 1);
		free (namez[i]);
	}
}

PHP_FUNCTION(libvirt_list_active_networks)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	int count;
	int i;

	GET_CONNECTION_FROM_ARGS("r", &zconn);
	array_init (return_value);

	count = virConnectNumOfNetworks (conn->conn);
	char *names[count];
	count = virConnectListNetworks (conn->conn, names, count);
	if (count < 0)
	{
		RETURN_FALSE;
	}

	for (i = 0; i < count; i++)
	{
		add_next_index_string (return_value, names[i], 1);
		free (names[i]);
	}
}

PHP_FUNCTION(libvirt_list_defined_networks)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	int count;
	int i;

	GET_CONNECTION_FROM_ARGS("r", &zconn);
	array_init (return_value);

	count = virConnectNumOfDefinedNetworks (conn->conn);
	if (count < 0)
	{
		RETURN_FALSE;
	}
	char *namez[count];
	count = virConnectListDefinedNetworks (conn->conn, namez, count);
	for (i = 0; i < count; i++)
	{
		add_next_index_string (return_value, namez[i], 1);
		free (namez[i]);
	}
}


PHP_FUNCTION(libvirt_domain_attach_device)
{
	php_libvirt_domain *domain = NULL;
	zval *zdomain;
	char *xml;
	int xml_len;
	long flags = 0;

	GET_DOMAIN_FROM_ARGS ("rs|l", &zdomain, &xml, &xml_len, &flags);

	if ((xml == NULL) || (xml_len < 1))
	{
		RETURN_FALSE;
	}

	if (virDomainAttachDeviceFlags (domain->domain, xml, flags) != 0)
	{
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_domain_detach_device)
{
	php_libvirt_domain *domain = NULL;
	zval *zdomain;
	char *xml;
	int xml_len;
	long flags = 0;

	GET_DOMAIN_FROM_ARGS ("rs|l", &zdomain, &xml, &xml_len, &flags);

	if ((xml == NULL) || (xml_len < 1))
	{
		RETURN_FALSE;
	}

	if (virDomainDetachDeviceFlags (domain->domain, xml, flags) != 0)
	{
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_node_device_destroy)
{
	php_libvirt_node_device *device = NULL;
	zval *zdevice;

	GET_DEVICE_FROM_ARGS ("r", &zdevice);

	if (virNodeDeviceDestroy (device->device) != 0)
	{
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_node_device_dettach)
{
	php_libvirt_node_device *device = NULL;
	zval *zdevice;

	GET_DEVICE_FROM_ARGS ("r", &zdevice);

	if (virNodeDeviceDettach (device->device) != 0)
	{
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_node_device_get_name)
{
	php_libvirt_node_device *device = NULL;
	zval *zdevice;
	const char *name;

	GET_DEVICE_FROM_ARGS ("r", &zdevice);

	name = virNodeDeviceGetName (device->device);
	if (name == NULL)
	{
		RETURN_FALSE;
	}

	RETURN_STRING (name, 1);
}

PHP_FUNCTION(libvirt_node_device_get_parent)
{
	php_libvirt_node_device *device = NULL;
	zval *zdevice;
	const char *parent;

	GET_DEVICE_FROM_ARGS ("r", &zdevice);

	parent = virNodeDeviceGetParent (device->device);
	if (parent == NULL)
	{
		RETURN_FALSE;
	}

	RETURN_STRING (parent, 1);
}

PHP_FUNCTION(libvirt_node_device_get_xml_desc)
{
	php_libvirt_node_device *device = NULL;
	zval *zdevice;
	char *xml;
	char *xml_out;

	GET_DEVICE_FROM_ARGS ("r|l", &zdevice);
	
	xml = virNodeDeviceGetXMLDesc (device->device, 0);
	if (xml == NULL)
	{
		RETURN_FALSE;
	}

	RECREATE_STRING_WITH_E (xml_out, xml);

	RETURN_STRING (xml_out, 0);
}

PHP_FUNCTION(libvirt_node_device_lookup_by_name)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	php_libvirt_node_device *res_device;
	virNodeDevice *device = NULL;
	char *name;
	int name_len;

	GET_CONNECTION_FROM_ARGS ("rs", &zconn, &name, &name_len);

	if ((name == NULL) || (name_len < 1))
	{
		RETURN_FALSE;
	}

	device = virNodeDeviceLookupByName (conn->conn, name);
	if (device == NULL)
	{
		RETURN_FALSE;
	}

	res_device = emalloc (sizeof (php_libvirt_node_device));
	res_device->device = device;
	res_device->conn = conn;

	ZEND_REGISTER_RESOURCE (return_value, res_device, le_libvirt_host_device);
}

PHP_FUNCTION(libvirt_node_device_num_of_caps)
{

	php_libvirt_node_device *device = NULL;
	zval *zdevice;

	GET_DEVICE_FROM_ARGS ("r", &zdevice);

	RETURN_LONG (virNodeDeviceNumOfCaps (device->device));
}

PHP_FUNCTION(libvirt_node_device_reattach)
{
	php_libvirt_node_device *device = NULL;
	zval *zdevice;

	GET_DEVICE_FROM_ARGS ("r", &zdevice);

	if (virNodeDeviceReAttach (device->device) != 0)
	{
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_node_device_reset)
{
	php_libvirt_node_device *device = NULL;
	zval *zdevice;

	GET_DEVICE_FROM_ARGS ("r", &zdevice);

	if (virNodeDeviceReset (device->device) != 0)
	{
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_list_node_device)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	char *caps = NULL;
	int caps_len;
	int count;
	int i;

	GET_CONNECTION_FROM_ARGS ("r|s", &zconn, &caps, &caps_len);

	if ((count = virNodeNumOfDevices (conn->conn, caps, 0)) < 0)
	{
		RETURN_FALSE;
	}

	char *names[count];
	count = virNodeListDevices (conn->conn, caps, names, count, 0);

	array_init (return_value);
	for (i = 0; i < count; i++)
	{
		add_next_index_string (return_value, names[i], 1);
		free (names[i]);
	}
}

PHP_FUNCTION(libvirt_get_node_device_count)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	char *caps = NULL;
	int caps_len;
	int rst;

	GET_CONNECTION_FROM_ARGS ("r|sl", &zconn, &caps, &caps_len);

	if ((rst = virNodeNumOfDevices (conn->conn, caps, 0)) < 0)
	{
		RETURN_FALSE;
	}

	RETURN_LONG (rst);
}

PHP_FUNCTION(libvirt_list_interfaces)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	int count;
	int i;

	GET_CONNECTION_FROM_ARGS ("r", &zconn);

	array_init (return_value);

	count = virConnectNumOfInterfaces (conn->conn);
	char *names[count];
	count = virConnectListInterfaces (conn->conn, names, count);
	if (count < 0)
	{
		RETURN_FALSE;
	}
	for (i = 0; i < count; i++)
	{
		add_next_index_string (return_value, names[i], 1);
		free (names[i]);
	}

	count = virConnectNumOfDefinedInterfaces (conn->conn);
	char *namez[count];
	count = virConnectListInterfaces (conn->conn, namez, count);
	if (count < 0)
	{
		RETURN_FALSE;
	}
	for (i = 0; i < count; i++)
	{
		add_next_index_string (return_value, namez[i], 1);
		free (namez[i]);
	}
}

PHP_FUNCTION(libvirt_list_active_interfaces)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	int count;
	int i;

	GET_CONNECTION_FROM_ARGS ("r", &zconn);

	array_init (return_value);

	count = virConnectNumOfInterfaces (conn->conn);
	char *names[count];
	count = virConnectListInterfaces (conn->conn, names, count);
	if (count < 0)
	{
		RETURN_FALSE;
	}
	for (i = 0; i < count; i++)
	{
		add_next_index_string (return_value, names[i], 1);
		free (names[i]);
	}
}

PHP_FUNCTION(libvirt_list_defined_interfaces)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	int count;
	int i;

	GET_CONNECTION_FROM_ARGS ("r", &zconn);

	array_init (return_value);

	count = virConnectNumOfDefinedInterfaces (conn->conn);
	char *namez[count];
	count = virConnectListDefinedInterfaces (conn->conn, namez, count);
	if (count < 0)
	{
		RETURN_FALSE;
	}
	for (i = 0; i < count; i++)
	{
		add_next_index_string (return_value, namez[i], 1);
		free (namez[i]);
	}
}

PHP_FUNCTION(libvirt_get_interface_count)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	int a;
	int b;

	GET_CONNECTION_FROM_ARGS ("r", &zconn);

	a = virConnectNumOfInterfaces (conn->conn);
	if (a < 0)
	{
		RETURN_FALSE;
	}

	b = virConnectNumOfDefinedInterfaces (conn->conn);
	if (b < 0)
	{
		RETURN_FALSE;
	}

	RETURN_LONG (a + b);
}

PHP_FUNCTION(libvirt_get_active_interface_count)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	int count;

	GET_CONNECTION_FROM_ARGS ("r", &zconn);

	count = virConnectNumOfInterfaces (conn->conn);
	if (count < 0)
	{
		RETURN_FALSE;
	}
	RETURN_LONG (count);
}

PHP_FUNCTION(libvirt_get_defined_interface_count)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	int count;

	GET_CONNECTION_FROM_ARGS ("r", &zconn);

	count = virConnectNumOfDefinedInterfaces (conn->conn);
	if (count < 0)
	{
		RETURN_FALSE;
	}
	RETURN_LONG (count);
}

PHP_FUNCTION(libvirt_interface_create)
{
	php_libvirt_interface *interface = NULL;
	zval *zinterface;

	GET_INTERFACE_FROM_ARGS("r", &zinterface);

	if (virInterfaceCreate (interface->interface, 0) != 0)
	{
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_interface_define_xml)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	virInterfacePtr interface;
	php_libvirt_interface *res_interface;
	char *xml;
	int xml_len;

	GET_CONNECTION_FROM_ARGS ("rs", &zconn, &xml, &xml_len);
	if ((xml == NULL) || (xml_len < 1)) 
	{
		RETURN_FALSE;
	}

	interface = virInterfaceDefineXML (conn->conn, xml, 0);

	res_interface = emalloc (sizeof (php_libvirt_interface));

	res_interface->interface = interface;
	res_interface->conn = conn;

	ZEND_REGISTER_RESOURCE (return_value, res_interface, le_libvirt_interface);
}

PHP_FUNCTION(libvirt_interface_get_connect)
{
	php_libvirt_interface *interface = NULL;
	php_libvirt_connection *conn;
	zval *zinterface;

	GET_INTERFACE_FROM_ARGS ("r", &zinterface);

	conn = interface->conn;
	if (conn == NULL)
	{
		RETURN_FALSE;
	}
	
	RETURN_RESOURCE (conn->resource_id);
}

PHP_FUNCTION(libvirt_interface_get_mac_string)
{
	php_libvirt_interface *interface = NULL;
	zval *zinterface;
	const char *mac;

	GET_INTERFACE_FROM_ARGS ("r", &zinterface);
	mac = virInterfaceGetMACString (interface->interface);
	if (mac == NULL)
	{
		RETURN_FALSE;
	}
	RETURN_STRING (mac, 1);
}

PHP_FUNCTION(libvirt_interface_get_name)
{
	php_libvirt_interface *interface = NULL;
	zval *zinterface;
	const char *name;

	GET_INTERFACE_FROM_ARGS ("r", &zinterface);
	name = virInterfaceGetName (interface->interface);
	if (name == NULL)
	{
		RETURN_FALSE;
	}
	RETURN_STRING (name, 1);
}

PHP_FUNCTION(libvirt_interface_get_xml_desc)
{
	php_libvirt_interface *interface = NULL;
	zval *zinterface;
	long flags = 0;
	char *xml;
	char *xml_out;

	GET_INTERFACE_FROM_ARGS ("r|l", &zinterface, &flags);

	xml = virInterfaceGetXMLDesc (interface->interface, (unsigned int)flags);
	if (xml == NULL)
	{
		RETURN_FALSE;
	}

	RECREATE_STRING_WITH_E (xml_out, xml);

	RETURN_STRING (xml_out, 1);
}

PHP_FUNCTION(libvirt_interface_lookup_by_mac_string)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	virInterfacePtr interface;
	php_libvirt_interface *res_interface;
	char *val;
	int len;
	
	GET_CONNECTION_FROM_ARGS ("rs", &zconn, &val, &len);
	if ((val == NULL) || (len < 1))
	{
		RETURN_FALSE;
	}

	interface = virInterfaceLookupByMACString (conn->conn, val);
	if (interface == NULL)
	{
		RETURN_FALSE;
	}

	res_interface = emalloc (sizeof (php_libvirt_interface));
	res_interface->interface = interface;
	res_interface->conn = conn;

	ZEND_REGISTER_RESOURCE (return_value, res_interface, le_libvirt_interface);
}

PHP_FUNCTION(libvirt_interface_lookup_by_name)
{
	php_libvirt_connection *conn = NULL;
	zval *zconn;
	virInterfacePtr interface;
	php_libvirt_interface *res_interface;
	char *val;
	int len;
	
	GET_CONNECTION_FROM_ARGS ("rs", &zconn, &val, &len);
	if ((val == NULL) || (len < 1))
	{
		RETURN_FALSE;
	}

	interface = virInterfaceLookupByName (conn->conn, val);
	if (interface == NULL)
	{
		RETURN_FALSE;
	}

	res_interface = emalloc (sizeof (php_libvirt_interface));
	res_interface->interface = interface;
	res_interface->conn = conn;

	ZEND_REGISTER_RESOURCE (return_value, res_interface, le_libvirt_interface);
}

PHP_FUNCTION(libvirt_interface_undefine)
{
	php_libvirt_interface *interface;
	zval *zinterface;

	GET_INTERFACE_FROM_ARGS ("r", &zinterface);

	if (virInterfaceUndefine (interface->interface) != 0)
	{
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_interface_is_active)
{
	php_libvirt_interface *interface;
	zval *zinterface;

	GET_INTERFACE_FROM_ARGS ("r", &zinterface);

	RETURN_LONG (virInterfaceIsActive (interface->interface));
}

PHP_FUNCTION(libvirt_domain_has_current_snapshot)
{
	php_libvirt_domain *domain = NULL;
	zval *zdomain;

	GET_DOMAIN_FROM_ARGS ("r", &zdomain);

	RETURN_LONG (virDomainHasCurrentSnapshot (domain->domain, 0));
}

PHP_FUNCTION(libvirt_domain_revert_to_snapshot)
{
	php_libvirt_domain_snapshot *snapshot = NULL;
	zval *zsnapshot;

	GET_SNAPSHOT_FROM_ARGS ("r", &zsnapshot);

	if (virDomainRevertToSnapshot (snapshot->snapshot, 0) != 0)
	{
		RETURN_FALSE;
	}
	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_domain_snapshot_create_xml)
{
	php_libvirt_domain *domain = NULL;
	zval *zdomain;
	php_libvirt_domain_snapshot *res_snapshot;
	virDomainSnapshotPtr snapshot;
	char *val;
	int len;

	GET_DOMAIN_FROM_ARGS ("rs", &zdomain, &val, &len);
	if ((val == NULL) || (len < 1))
	{
		RETURN_FALSE;
	}

	snapshot = virDomainSnapshotCreateXML (domain->domain, val, 0);
	if (snapshot == NULL)
	{
		RETURN_FALSE;
	}

	res_snapshot = emalloc (sizeof (php_libvirt_domain_snapshot));
	res_snapshot->snapshot = snapshot;

	ZEND_REGISTER_RESOURCE (return_value, res_snapshot, le_libvirt_domain_snapshot);
}

PHP_FUNCTION(libvirt_domain_snapshot_current)
{
	php_libvirt_domain *domain = NULL;
	zval *zdomain;
	php_libvirt_domain_snapshot *res_snapshot;
	virDomainSnapshotPtr snapshot;
	virErrorPtr err;


	GET_DOMAIN_FROM_ARGS ("r", &zdomain);

	virResetLastError ();
	snapshot = virDomainSnapshotCurrent (domain->domain, 0);
	if (snapshot == NULL)
	{
		RETURN_FALSE;
	}
	err = virGetLastError (); 
	if (err->code == VIR_ERR_NO_DOMAIN_SNAPSHOT)
	{
		RETURN_NULL();
	}

	res_snapshot = emalloc (sizeof (php_libvirt_domain_snapshot));
	res_snapshot->snapshot = snapshot;

	ZEND_REGISTER_RESOURCE (return_value, res_snapshot, le_libvirt_domain_snapshot);
}

PHP_FUNCTION(libvirt_domain_snapshot_delete)
{
	php_libvirt_domain_snapshot *snapshot = NULL;
	zval *zsnapshot;
	long flags = 0;

	GET_SNAPSHOT_FROM_ARGS ("rl", &zsnapshot, &flags);

	if (virDomainSnapshotDelete (snapshot->snapshot, (unsigned int)flags) != 0)
	{
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

PHP_FUNCTION(libvirt_domain_snapshot_get_xml_desc)
{
	php_libvirt_domain_snapshot *snapshot = NULL;
	zval *zsnapshot;
	char *xml;
	char *xml_out;

	GET_SNAPSHOT_FROM_ARGS ("r", &snapshot);

	xml = virDomainSnapshotGetXMLDesc (snapshot->snapshot, 0);
	if (xml == NULL)
	{
		RETURN_FALSE;
	}

	RECREATE_STRING_WITH_E (xml_out, xml);

	RETURN_STRING (xml_out, 0);
}

PHP_FUNCTION(libvirt_domain_snapshot_list)
{
	php_libvirt_domain *domain = NULL;
	zval *zdomain;
	int count;
	int i;

	GET_DOMAIN_FROM_ARGS ("r", &zdomain);

	count = virDomainSnapshotNum (domain->domain, 0);
	char *names[count];
	count = virDomainSnapshotListNames (domain->domain, names, count, 0);
	if (count != 0)
	{
		RETURN_FALSE;
	}

	array_init (return_value);
	for (i = 0; i < count; i++)
	{
		add_next_index_string (return_value, names[i], 1);
		free (names[i]);
	}
}

PHP_FUNCTION(libvirt_domain_snapshot_lookup_by_name)
{
	php_libvirt_domain *domain = NULL;
	zval *zdomain;
	php_libvirt_domain_snapshot *res_snapshot;
	virDomainSnapshotPtr snapshot;
	char *name;
	int len;
	virErrorPtr err;

	GET_DOMAIN_FROM_ARGS ("rs", &zdomain, &name, &len);
	if ((name == NULL) || (len < 1))
	{
		RETURN_FALSE;
	}

	virResetLastError ();
	snapshot = virDomainSnapshotLookupByName (domain->domain, name, 0);
	if (snapshot == NULL)
	{
		RETURN_FALSE;
	}
	err = virGetLastError ();
	if (err->code == VIR_ERR_NO_DOMAIN_SNAPSHOT)
	{
		RETURN_NULL();
	}

	res_snapshot = emalloc (sizeof (php_libvirt_domain_snapshot));
	res_snapshot->snapshot = snapshot;

	ZEND_REGISTER_RESOURCE (return_value, res_snapshot, le_libvirt_domain_snapshot);
}

PHP_FUNCTION(libvirt_domain_snapshot_count)
{
	php_libvirt_domain *domain = NULL;
	zval *zdomain;
	int count;

	GET_DOMAIN_FROM_ARGS ("r", &zdomain);

	count = virDomainSnapshotNum(domain->domain, 0);
	if (count != 0)
	{
		RETURN_FALSE;
	}

	RETURN_LONG (count);
}
