/*
 * GMC��GMC_??���쐬���邽�߂�SQL
 */
drop user &1 cascade;
create user &1 identified by &2;
grant connect to &1;
grant resource to &1;
grant dba to &1 with admin option;

exit
