spool TokuReki60.log
set echo on
set trimspool on
set timing on
delete from TOKUSYO_RIREKI where TREKI_DATE < to_date(20090201,'yyyymmdd');
commit;
delete from TOKUSYO_RIREKI where TREKI_DATE <= to_date(20090227,'yyyymmdd');
commit;
spool off
exit
