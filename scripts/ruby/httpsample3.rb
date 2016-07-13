#!/usr/bin/ruby
# coding: utf-8

$KCODE = 'utf-8'

require 'uri'
# require 'net/http'
require 'net/https'
require 'rexml/document'

def http_request(method, uri, query_hash={})
	#query = query_hash.map{|k, v| "#{k}=#{v}"}.join('&')        #ハッシュをオプションの書式に変換
	query = URI.encode_www_form(query_hash)
#puts query
	query_escaped = URI.escape(query)
#puts query_escaped

	uri_parsed = URI.parse(uri)
	http = Net::HTTP.new(uri_parsed.host, 443)
	http.use_ssl = true
	http.verify_mode = OpenSSL::SSL::VERIFY_NONE
	http.verify_depth = 5

	case method.downcase!
	when 'get'
		return http.get(uri_parsed.path + '?' + query_escaped).body
	when 'post'
		return http.post(uri_parsed.path, query_escaped).body
	end
end
 
# puts http_request('GET', 'https://papimo.jp/api/v1/hall/installed.xml', {:key => 'GNCloudServiceConnector', :store => '80095009', :device => 'IFBOX', :version => '1.00'})

puts http_request('GET', 'https://papimo.jp/api/v1/hall/installedReserve.xml', {:key => 'GNCloudServiceConnector', :store => '80095009', :device => 'IFBOX', :version => '1.00'})

# puts http_request('GET', 'https://papimo.jp/api/v1/hall/installedStatus.xml', {:key => 'GNCloudServiceConnector', :store => '80095009', :device => 'IFBOX', :version => '小.島 良.太', :installedId => '16586', :status => '0'})
