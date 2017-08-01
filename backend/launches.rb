#!/usr/bin/env ruby
#
# Lists upcoming rocket launches and their status when typing `launches`
# Select an entry to open livestream or information websites
# Data provided by the amazing LaunchLibrary
#

require 'json'
require 'open-uri'

ENDPOINT = 'https://launchlibrary.net/1.2/launch/next/10'

class Launches
  def run
    STDERR.print '> '
    while input = gets
      input = input.chomp if input
      results = if input == 'launches'
        get_launch_data
      else
        []
      end
      puts serialize(results)
      STDOUT.flush
      STDERR.print '> '
    end
  end

  private

  GREEN   = 1
  RED     = 2
  SUCCESS = 3
  FAILURE = 4

  def get_image_for_status(entry)
    icon = case entry['status']
    when GREEN
      'rocket-green.png'
    when RED
      'rocket-red.png'
    when SUCCESS
      'rocket-success.png'
    when FAILURE
      'rocket-failure.png'
    end
    icon = 'rocket-hold' if entry['inhold'] == 1
    File.join(File.expand_path(File.dirname(__FILE__)), "icons/#{icon}")
  end

  def get_url(entry)
    if entry['vidURL']
      "xdg-open #{entry['vidURL']}"
    elsif entry['vidURLs'] && entry['vidURLs'].size > 0
      "xdg-open #{entry['vidURLs'][0]}"
    elsif entry['infoURL']
      "xdg-open #{entry['infoURL']}"
    elsif entry['infoURLs'] && entry['infoURLs'].size
      "xdg-open #{entry['infoURLs'][0]}"
    else
      ''
    end
  end

  def get_launch_data
    launches = []
    open ENDPOINT do |fd|
      data = JSON.parse!(fd.read)['launches']
      data.each do |entry|
        launches << {
          :name => entry['name'],
          :description => entry['net'],
          :icon => get_image_for_status(entry),
          :exec => get_url(entry)
        }
      end
    end
    return launches
  end

  def serialize(results)
    {
      :backend => 'launches',
      :version => '1.0.0',
      :priority => 9,
      :results => results
    }.to_json
  end
end

Launches.new.run if __FILE__ == $0
