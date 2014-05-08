require 'socket'

class Client
  REGISTER_CMD = 0x40
  CL_PLACE_BOMB_CMD = 0x80
  MOVE_CMD = 0xC0
  UNREGISTER_CMD = 0x20

  attr_accessor :ip, :port, :adr, :exit
  attr_reader :socket, :output

  def initialize(ip, port, output = false)
    @ip, @port, @output = ip, port, output
    @exit = false
    @socket = UDPSocket.new
    @adr = Addrinfo.udp(ip, port)
  end

  def do_register
    send REGISTER_CMD
    new_port, _ = socket.recvfrom 2
    self.port = (new_port[0].ord << 8) | new_port[1].ord
    socket.close
    @socket = UDPSocket.new
    self.adr = Addrinfo.udp(ip, port)
    send REGISTER_CMD
  end

  def main_loop
    while(!exit) do
      move_msg = MOVE_CMD | (rand(4) + 1)
      send move_msg
      send CL_PLACE_BOMB_CMD if rand(100) < 1
      sleep(rand(60..100) / 100.0)
      rs, _, _ = IO.select([socket], nil, nil, 0)
      while rs && rs[0]
        msg, _ = socket.recvfrom 8192
        puts msg.each_byte.map{ |b| '0x' + b.to_s(16).rjust(2, '0') }.join(' ') if output
        rs, _, _ = IO.select([socket], nil, nil, 0)
      end
    end
  end

  def do_unregister
    send UNREGISTER_CMD
  end

  def send(msg)
    binary_msg = msg.chr
    socket.send binary_msg, 0, adr
  end
end
