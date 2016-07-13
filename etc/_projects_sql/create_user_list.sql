/*
 * GMCで始まるユーザーを一覧にするSQL
 */
set pagesize 0
set trimspool on
set echo off
set feedback off
spool userlist.txt
select username from dba_users where username like 'GMC%' order by username;
spool off

exit
