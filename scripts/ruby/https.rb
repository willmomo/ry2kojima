#!/usr/bin/ruby
# coding: utf-8


# >ruby -r openssl -e 'p OpenSSL::X509::DEFAULT_CERT_FILE'
# 出力結果
# "C:/Users/Luis/Code/openknapsack/knap-build/var/knapsack/software/x86-windows/openssl/1.0.0k/ssl/cert.pem"

require 'net/https'
https = Net::HTTP.new('papimo.jp', 443)
https.use_ssl = true
https.ca_file = 'C:/Users/Luis/Code/openknapsack/knap-build/var/knapsack/software/x86-windows/openssl/1.0.0k/ssl/cert.pem'
#https.verify_mode = OpenSSL::SSL::VERIFY_PEER
https.verify_mode = OpenSSL::SSL::VERIFY_NONE
https.verify_depth = 5
https.start {
  response = https.get('/api/v1/hall/installed.xml')
  puts response.body
}
