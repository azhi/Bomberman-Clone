require_relative 'ruby_client'

class StressTester
  def self.test(n, timeout)
    instance = self.new(n)
    instance.start
    sleep(timeout)
    instance.stop
  end

  def initialize(n)
    @n = n
    @threads = []
    @clients = []
  end

  def start
    (@n-1).times do
      add_thread
    end
    add_thread(true)
  end

  def add_thread(output = false)
    @threads << Thread.new do
      c = Client.new('127.0.0.1', 5555, output)
      @clients << c
      c.do_register
      c.main_loop
      c.do_unregister
    end
  end

  def stop
    @clients.each{ |c| c.exit = true }
    @threads.each(&:join)
  end
end
