#!/usr/bin/ruby
# coding: utf-8

$KCODE = 'utf-8'

require 'uri'
require 'net/http'
require 'rexml/document'

#----------------------------------------------------------------------
# utf-8のソースの出力をsjisに化かす
#----------------------------------------------------------------------
if RUBY_PLATFORM =~ /mswin(?!ce)|mingw|cygwin|bccwin/
	require 'kconv'
	class StdoutHook
		def write(str)
			STDOUT.write NKF.nkf("--ic=UTF-8 --oc=Windows-31J", str.to_s)
		end
		$> = new
	end
end

#----------------------------------------------------------------------
# PAPIMO 設置台情報取得/設置台予約設定情報取得 API の簡易実装
# @param key アクセスキー
# @param store 店舗ID(加盟店コード)
# @param device 機器名称(ログ保存用)
# @param version 機器バージョン(ログ保存用)
# @retval true HTTPステータスコードが200のとき、true
# @retval false HTTPステータスコードが200以外のとき、何らかの通信異常のときfalse
#----------------------------------------------------------------------
def get_installedUpdateInfo(key, store, device, version)
	query_hash = {:key=>key, :store=>store, :device=>device, :version=>version}
	query = query_hash.map{|k, v| "#{k}=#{v}"}.join('&')
	query_escaped = URI.escape(query)
	p query_escaped
	uri = 'http://papimo.jp/api/v1/hall/installedUpdateInfo.xml'
	uri_parsed = URI.parse(uri)
	begin
		http = Net::HTTP.new(uri_parsed.host)
		res = http.get(uri_parsed.path + '?' + query_escaped)
		# p res.code
		p res.body
		res.code == "200"
	rescue
		false
	end
end

#----------------------------------------------------------------------
# PAPIMO 設置台情報取得/設置台予約設定情報取得 API の簡易実装
# @param key アクセスキー
# @param store 店舗ID(加盟店コード)
# @param device 機器名称(ログ保存用)
# @param version 機器バージョン(ログ保存用)
# @param get_reserve trueのとき、installedReserve.xml APIを使用
# @retval true HTTPステータスコードが200のとき、true
# @retval false HTTPステータスコードが200以外のとき、何らかの通信異常のときfalse
#----------------------------------------------------------------------
def get_installed(key, store, device, version, get_reserve)
	query_hash = {:key=>key, :store=>store, :device=>device, :version=>version}
	query = query_hash.map{|k, v| "#{k}=#{v}"}.join('&')
	query_escaped = URI.escape(query)
	p query_escaped
	uri = (get_reserve) ? 'http://papimo.jp/api/v1/hall/installed.xml' : 'http://papimo.jp/api/v1/hall/installedReserve.xml'
	uri_parsed = URI.parse(uri)
	begin
		http = Net::HTTP.new(uri_parsed.host)
		res = http.get(uri_parsed.path + '?' + query_escaped)
		# p res.code
		# p res.body
		res.code == "200"
	rescue
		false
	end
end

#----------------------------------------------------------------------
# PAPIMO 設置台情報状態通知 API の簡易実装
# @param key アクセスキー
# @param store 店舗ID(加盟店コード)
# @param device 機器名称(ログ保存用)
# @param version 機器バージョン(ログ保存用)
# @param installedId 設定情報ID
# @param status 設置台情報の処理結果
# @retval true HTTPステータスコードが200のとき、true
# @retval false HTTPステータスコードが200以外のとき、何らかの通信異常のときfalse
#----------------------------------------------------------------------
def set_installedStatus(key, store, device, version, installedId, status)
	query_hash = {:key=>key, :store=>store, :device=>device, :version=>version, :installedId=>installedId, :status=>status}
	query = query_hash.map{|k, v| "#{k}=#{v}"}.join('&')
	query_escaped = URI.escape(query)
	p query_escaped
	uri = 'http://papimo.jp/api/v1/hall/installedStatus.xml'
	uri_parsed = URI.parse(uri)
	begin
		http = Net::HTTP.new(uri_parsed.host)
		res = http.get(uri_parsed.path + '?' + query_escaped)
		# p res.code
		p res.body
		res.code == "200"
	rescue
		false
	end
end

p '日本語'

p get_installedUpdateInfo('GNCloudServiceConnector', '80095009', 'IFBOX', 'v1.000')
p get_installed('GNCloudServiceConnector', '80095009', 'IFBOX', 'v1.000', false)
p get_installed('GNCloudServiceConnector', '80095009', 'IFBOX', 'v1.000', true)
p set_installedStatus('GNCloudServiceConnector', '80095009', 'IFBOX', 'v1.000', 16586, 0)
