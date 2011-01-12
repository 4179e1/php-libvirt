#ifndef PHP_LIBVIRT_H
#define PHP_LIBVIRT_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include <libvirt/libvirt.h>

ZEND_BEGIN_MODULE_GLOBALS(libvirt)
	char *last_error;
	zend_bool longlong_to_string_ini;
ZEND_END_MODULE_GLOBALS(libvirt)

#ifdef ZTS
#define LIBVIRT_G(v) TSRMG(libvirt_globals_id, zend_libvirt_globals *, v)
#else
#define LIBVIRT_G(v) (libvirt_globals.v)
#endif

#define PHP_LIBVIRT_WORLD_VERSION "0.4"
#define PHP_LIBVIRT_WORLD_EXTNAME "libvirt"


typedef struct _php_libvirt_connection {
    virConnectPtr conn;
    long resource_id;
} php_libvirt_connection;

typedef struct _php_libvirt_domain {
    virDomainPtr domain;
#ifdef HAVE_GETCONNECT
    php_libvirt_connection* conn;
#endif /* HAVE_GETCONNECT */
} php_libvirt_domain;

typedef struct _php_libvirt_storagepool {
    virStoragePoolPtr pool;
#ifdef HAVE_GETCONNECT
    php_libvirt_connection* conn;
#endif /* HAVE_GETCONNECT */
} php_libvirt_storagepool;

typedef struct _php_libvirt_volume {
    virStorageVolPtr volume;
} php_libvirt_volume;

typedef struct _php_libvirt_network {
	virNetworkPtr network;
#ifdef HAVE_GETCONNECT
	php_libvirt_connection *conn;
#endif /* HAVE_GETCONNECT */
} php_libvirt_network;

typedef struct _php_libivrt_node_device {
	virNodeDevicePtr device;
#ifdef HAVE_GETCONNECT
	php_libvirt_connection *conn;
#endif /* HAVE_GETCONNECT */
} php_libvirt_node_device;

typedef struct _php_libvirt_interface {
	virInterfacePtr interface;
#ifdef HAVE_GETCONNECT
	php_libvirt_connection *conn;
#endif /* HAVE_GETCONNECT */
} php_libvirt_interface;

typedef struct _php_libvirt_domain_snapshot {
	virDomainSnapshotPtr snapshot;
} php_libvirt_domain_snapshot;

typedef struct _php_libvirt_cred_value {
	int count;
	int	type;
	char *result;
	unsigned int	resultlen;
} php_libvirt_cred_value;


#define PHP_LIBVIRT_CONNECTION_RES_NAME "Libvirt connection"
#define PHP_LIBVIRT_DOMAIN_RES_NAME "Libvirt domain"
#define PHP_LIBVIRT_STORAGEPOOL_RES_NAME "Libvirt storagepool"
#define PHP_LIBVIRT_VOLUME_RES_NAME "Libvirt volume"
#define PHP_LIBVIRT_NETWORK_RES_NAME "Libvirt network"
#define PHP_LIBVIRT_HOST_DEVICE_RES_NAME "Libvirt device"
#define PHP_LIBVIRT_INTERFACE_RES_NAME "Libvirt interface"
#define PHP_LIBVIRT_DOMAIN_SNAPSHOT_RES_NAME "Libvirt snapshot"

PHP_MINIT_FUNCTION(libvirt);
PHP_MSHUTDOWN_FUNCTION(libvirt);
PHP_RINIT_FUNCTION(libvirt);
PHP_RSHUTDOWN_FUNCTION(libvirt);
PHP_MINFO_FUNCTION(libvirt);

PHP_FUNCTION(libvirt_get_last_error);
PHP_FUNCTION(libvirt_connect);
PHP_FUNCTION(libvirt_get_hostname);
PHP_FUNCTION(libvirt_node_get_info);
PHP_FUNCTION(libvirt_get_capabilities);
PHP_FUNCTION(libvirt_get_max_vcpus);
PHP_FUNCTION(libvirt_node_get_free_memory);
PHP_FUNCTION(libvirt_get_type);
PHP_FUNCTION(libvirt_get_version);
PHP_FUNCTION(libvirt_get_lib_version);
PHP_FUNCTION(libvirt_get_uri);
PHP_FUNCTION(libvirt_get_active_domain_count);
PHP_FUNCTION(libvirt_get_inactive_domain_count);
PHP_FUNCTION(libvirt_get_domain_count);
PHP_FUNCTION(libvirt_get_domain_state_string);
PHP_FUNCTION(libvirt_domain_lookup_by_name);
PHP_FUNCTION(libvirt_domain_get_xml_desc);
PHP_FUNCTION(libvirt_domain_get_info);
PHP_FUNCTION(libvirt_list_domains);
PHP_FUNCTION(libvirt_list_domain_names);
PHP_FUNCTION(libvirt_list_active_domain_names);
PHP_FUNCTION(libvirt_list_inactive_domain_names);
PHP_FUNCTION(libvirt_domain_get_uuid);
PHP_FUNCTION(libvirt_domain_get_uuid_string);
PHP_FUNCTION(libvirt_domain_get_name);
PHP_FUNCTION(libvirt_list_active_domains);
PHP_FUNCTION(libvirt_list_inactive_domains);
PHP_FUNCTION(libvirt_domain_get_id);
PHP_FUNCTION(libvirt_domain_lookup_by_id);
PHP_FUNCTION(libvirt_domain_lookup_by_uuid);
PHP_FUNCTION(libvirt_domain_lookup_by_uuid_string);
PHP_FUNCTION(libvirt_domain_destroy);
PHP_FUNCTION(libvirt_domain_create);
PHP_FUNCTION(libvirt_domain_resume);
PHP_FUNCTION(libvirt_domain_shutdown);
PHP_FUNCTION(libvirt_domain_suspend);
PHP_FUNCTION(libvirt_domain_undefine);
PHP_FUNCTION(libvirt_domain_reboot);
PHP_FUNCTION(libvirt_domain_define_xml);
PHP_FUNCTION(libvirt_domain_create_xml);
PHP_FUNCTION(libvirt_domain_memory_peek);
#if LIBVIR_VERSION_NUMBER>=7005
PHP_FUNCTION(libvirt_domain_memory_stats);
#endif
PHP_FUNCTION(libvirt_domain_block_stats);
PHP_FUNCTION(libvirt_domain_interface_stats);
PHP_FUNCTION(libvirt_domain_get_autostart);
PHP_FUNCTION(libvirt_domain_set_autostart);
PHP_FUNCTION(libvirt_domain_is_active);
PHP_FUNCTION(libvirt_version);
#ifdef HAVE_GETCONNECT
PHP_FUNCTION(libvirt_domain_get_connect);
#endif /* HAVE_GETCONNECT */
PHP_FUNCTION(libvirt_domain_migrate);
#if LIBVIR_VERSION_NUMBER>=7007
PHP_FUNCTION(libvirt_domain_get_job_info);
#endif
PHP_FUNCTION(libvirt_domain_migrate_to_uri);
PHP_FUNCTION(libvirt_list_storagepools);
PHP_FUNCTION(libvirt_list_active_storagepools);
PHP_FUNCTION(libvirt_list_inactive_storagepools);
PHP_FUNCTION(libvirt_get_storagepool_state_string);
PHP_FUNCTION(libvirt_storagepool_get_uuid_string);
PHP_FUNCTION(libvirt_storagepool_lookup_by_name);
PHP_FUNCTION(libvirt_storagepool_list_volumes);
PHP_FUNCTION(libvirt_storagepool_get_info);
PHP_FUNCTION(libvirt_storagepool_get_name);
PHP_FUNCTION(libvirt_storagepool_lookup_by_uuid_string);
PHP_FUNCTION(libvirt_storagepool_get_xml_desc);
PHP_FUNCTION(libvirt_storagepool_define_xml);
PHP_FUNCTION(libvirt_storagepool_undefine);
PHP_FUNCTION(libvirt_storagepool_create);
PHP_FUNCTION(libvirt_storagepool_destroy);
#ifdef HAVE_GETCONNECT
PHP_FUNCTION(libvirt_storagepool_get_connect);
#endif /* HAVE_GETCONNECT */
PHP_FUNCTION(libvirt_storagepool_is_active);
PHP_FUNCTION(libvirt_storagepool_get_volume_count);
PHP_FUNCTION(libvirt_storagepool_refresh);
PHP_FUNCTION(libvirt_storagepool_set_autostart);
PHP_FUNCTION(libvirt_storagepool_get_autostart);
PHP_FUNCTION(libvirt_get_storagepool_count);
PHP_FUNCTION(libvirt_get_active_storagepool_count);
PHP_FUNCTION(libvirt_get_inactive_storagepool_count);
PHP_FUNCTION(libvirt_storagevolume_lookup_by_name);
PHP_FUNCTION(libvirt_storagevolume_get_info);
PHP_FUNCTION(libvirt_storagevolume_get_xml_desc);
PHP_FUNCTION(libvirt_storagevolume_create_xml);
PHP_FUNCTION(libvirt_storagevolume_get_path);
PHP_FUNCTION(libvirt_storagevolume_get_name);
PHP_FUNCTION(libvirt_get_storagevolume_type_string);
PHP_FUNCTION(libvirt_list_networks);
PHP_FUNCTION(libvirt_list_active_networks);
PHP_FUNCTION(libvirt_list_inactive_networks);
PHP_FUNCTION(libvirt_get_network_count);
PHP_FUNCTION(libvirt_get_active_network_count);
PHP_FUNCTION(libvirt_get_inactive_network_count);
PHP_FUNCTION(libvirt_network_define_xml);
PHP_FUNCTION(libvirt_network_create);
PHP_FUNCTION(libvirt_network_destroy);
PHP_FUNCTION(libvirt_network_undefine);
PHP_FUNCTION(libvirt_network_get_autostart);
PHP_FUNCTION(libvirt_network_get_bridge_name);
#ifdef HAVE_GETCONNECT
PHP_FUNCTION(libvirt_network_get_connect);
#endif /* HAVE_GETCONNECT */
PHP_FUNCTION(libvirt_network_get_name);
PHP_FUNCTION(libvirt_network_get_uuid_string);
PHP_FUNCTION(libvirt_network_get_xml_desc);
PHP_FUNCTION(libvirt_network_is_active);
PHP_FUNCTION(libvirt_network_lookup_by_name);
PHP_FUNCTION(libvirt_network_lookup_by_uuid_string);
PHP_FUNCTION(libvirt_network_set_autostart);
PHP_FUNCTION(libvirt_domain_attach_device);
PHP_FUNCTION(libvirt_domain_detach_device);
PHP_FUNCTION(libvirt_node_device_destroy);
PHP_FUNCTION(libvirt_node_device_dettach);
PHP_FUNCTION(libvirt_node_device_get_name);
PHP_FUNCTION(libvirt_node_device_get_parent);
PHP_FUNCTION(libvirt_node_device_get_xml_desc);
PHP_FUNCTION(libvirt_node_device_lookup_by_name);
PHP_FUNCTION(libvirt_node_device_num_of_caps);
PHP_FUNCTION(libvirt_node_device_reattach);
PHP_FUNCTION(libvirt_node_device_reset);
PHP_FUNCTION(libvirt_list_node_devices);
PHP_FUNCTION(libvirt_get_node_device_count);

PHP_FUNCTION(libvirt_list_interfaces);
PHP_FUNCTION(libvirt_list_active_interfaces);
PHP_FUNCTION(libvirt_list_inactive_interfaces);
PHP_FUNCTION(libvirt_get_interface_count);
PHP_FUNCTION(libvirt_get_active_interface_count);
PHP_FUNCTION(libvirt_get_inactive_interface_count);
PHP_FUNCTION(libvirt_interface_create);
PHP_FUNCTION(libvirt_interface_define_xml);
PHP_FUNCTION(libvirt_interface_destroy);
#ifdef HAVE_GETCONNECT
PHP_FUNCTION(libvirt_interface_get_connect);
#endif /* HAVE_GETCONNECT */
PHP_FUNCTION(libvirt_interface_get_mac_string);
PHP_FUNCTION(libvirt_interface_get_name);
PHP_FUNCTION(libvirt_interface_get_xml_desc);
PHP_FUNCTION(libvirt_interface_lookup_by_mac_string);
PHP_FUNCTION(libvirt_interface_lookup_by_name);
PHP_FUNCTION(libvirt_interface_undefine);
PHP_FUNCTION(libvirt_interface_is_active);

PHP_FUNCTION(libvirt_domain_has_current_snapshot);
PHP_FUNCTION(libvirt_domain_revert_to_snapshot);
PHP_FUNCTION(libvirt_domain_snapshot_create_xml);
PHP_FUNCTION(libvirt_domain_snapshot_current);
PHP_FUNCTION(libvirt_domain_snapshot_delete);
PHP_FUNCTION(libvirt_domain_snapshot_get_xml_desc);
PHP_FUNCTION(libvirt_list_domain_snapshots);
PHP_FUNCTION(libvirt_domain_snapshot_lookup_by_name);
PHP_FUNCTION(libvirt_domain_snapshot_count);

extern zend_module_entry libvirt_module_entry;
#define phpext_libvirt_ptr &libvirt_module_entry

#endif
