/*
 * GMC‚âGMC_??‚ğì¬‚·‚é‚½‚ß‚ÌSQL
 */
drop user &1 cascade;
create user &1 identified by &2;
grant connect to &1;
grant resource to &1;
grant dba to &1 with admin option;

exit
