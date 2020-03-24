$:.unshift File.join( File.dirname( __FILE__ ), '..', 'lib')
require 'osc-ruby'

require 'rubygems'
require 'eventmachine'
require 'osc-ruby/em_server'
require 'serialport'

@server = OSC::EMServer.new( 5050 )
@sp = SerialPort.new("/dev/ttyACM0", 9600, 8, 1, SerialPort::NONE)

@server.add_method '/tidal' do | message |
  #puts "#{message.args}"
  cur_vals = message.to_a
  targetidx = cur_vals.index("target")
  if targetidx
    cur_target = cur_vals[targetidx + 1]
    @sp.putc(cur_target.to_i)
    @sp.flush()
  end

  puts "#{message.ip_address}:#{message.ip_port} -- #{message.address} -- #{message.to_a}"
end

@server.run





