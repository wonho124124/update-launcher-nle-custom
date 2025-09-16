using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace update_monitoring
{
    public class UpdateData
    {
        [JsonProperty("local_ip")]
        public string LocalHost { get; set; }
        [JsonProperty("model_nm")]
        public string ComputerName { get; set; }
        [JsonProperty("module")]
        public string ModuleName { get; set; }
        [JsonProperty("local_ver")]
        public string LocalVersion { get; set; }
        [JsonProperty("svr_ver")]
        public string ServerVersion { get; set; }
    }
        
    

    public class Program
    {
        
        static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                return;
            }

            string fullPath = args[0];
            if (File.Exists(fullPath))
            {
                UpdateData updateData = new UpdateData();
                updateData.LocalHost = GetIP4Address();
                updateData.ComputerName = System.Net.Dns.GetHostName();
                updateData.ModuleName = Path.GetFileNameWithoutExtension(fullPath);
                updateData.LocalVersion = FileVersionInfo.GetVersionInfo(fullPath).FileVersion;
                updateData.ServerVersion = args.Length > 1 ? args[1] : "";
                
                RestServiceAPI restService = new RestServiceAPI(Properties.Settings.Default.updateMonitoringUrl);
                string ret = restService.RestServiceExecute(updateData);

            }            
        }

        static public string GetIP4Address()
        {
            //string IP4Address = String.Empty;
            List<string> IP4Address = new List<string>();

            foreach (IPAddress IPA in Dns.GetHostAddresses(Dns.GetHostName()))
            {
                if (IPA.AddressFamily == AddressFamily.InterNetwork)
                {
                    IP4Address.Add(IPA.ToString());
                }
            }

            return IP4Address[0];
        }
    }
}
