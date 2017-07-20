#!/usr/bin/env ruby
#
# Lists *.desktop application entries using fuzzy matching
# Searches for $HOME/.local/share/applications and /usr/share/applications
# Returns a JSON representation of the matched entries
#

require 'json'

class String
  def underscore
    gsub(/::/, '/')
      .gsub(/([A-Z]+)([A-Z][a-z])/,'\1_\2')
      .gsub(/([a-z\d])([A-Z])/,'\1_\2')
      .tr('- ', '_')
      .downcase
  end
end

class ApplicationEntry
  def initialize(entry)
    @data = parse_entry entry
  end

  def method_missing(method_sym)
    @data[method_sym]
  end

  def data
    @data
  end

  private

  def parse_entry(entry)
    entry_data = {}
    section = ''
    entry.lines.each do |line|
      if /^\[(.+)\]$/ =~ line
        section = $1
      end
      next unless section == 'Desktop Entry'
      next unless /^.*=.*$/ =~ line.chomp
      prop, value = line.chomp.split('=', 2)
      next if /^X-[^-]+/ =~ prop
      next if /^.*\[.+\]$/ =~ prop
      entry_data[prop.underscore.to_sym] = cast_value(value)
    end
    entry_data
  end

  def cast_value(value)
    return unless value.respond_to? :downcase
    return false if value.downcase == "false"
    return true if value.downcase == "true"
    return value.to_f if /^\d+(?:[\.,]\d*)?$/ =~ value
    return value.split(';') if /^(?:[^;]+;)+$/ =~ value
    value
  end
end

class Backend
  def start
    apps = find_apps
    STDERR.print '> '
    while input = gets&.chomp
      if input.length >= 3
        regex = fuzzy_find(input&.downcase)
        filtered = apps.select do |app|
          app.name&.downcase =~ regex
        end.sort_by &:name
        puts serialize(filtered)
      end
      STDERR.print '> '
    end
  end

  private

  def fuzzy_find(search)
    start = search[0..-2]
    end_char = Regexp.escape search[-1]
    reg = start.split('').map { |s| "(#{Regexp.escape s}.*?)" }.join('')
    return Regexp.new "(.*)#{reg}(#{end_char})"
  end

  def find_apps
    apps = []
    home = Dir["#{ENV['HOME']}/.local/share/applications/*.desktop"]
    sys = Dir['/usr/share/applications/*.desktop']
    home.concat(sys).each do |desktop|
      data = File.read(desktop)
      app = ApplicationEntry.new(data)
      apps << app unless app.no_display
    end
    apps.uniq { |app| [app.name, app.exec] }
  end

  def serialize(filtered)
    filtered = filtered.map &:data
    
    {
      :backend => 'application',
      :version => '1.0.0',
      :priority => 2,
      :results => filtered
    }.to_json.gsub('\'', %q(\\\')).gsub('"', %q(\\"))
  end
end

Backend.new.start if __FILE__ == $0
