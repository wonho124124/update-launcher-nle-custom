using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using RestSharp;
using RestSharp.Serializers.Newtonsoft.Json;

namespace update_monitoring
{
    /// <summary>
    /// REST 서비스 기본 클래스
    /// </summary>
    public class BaseServiceAPI
    {
        
        protected Uri _uri; 
        protected string _baseUrl;
        public BaseServiceAPI(string url)
        {
            _uri = new Uri(url);
            _baseUrl = _uri.GetLeftPart(System.UriPartial.Authority);
        }
    }

    /// <summary>
    /// REST 서비스 실행 클래스
    /// 작업 진행 및 완료 상태 업데이트
    /// </summary>
    public class RestServiceAPI : BaseServiceAPI
    {        
        public RestServiceAPI(string url)
            : base(url)
        {
        }
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="obj"></param>
        /// <returns></returns>
        public string RestServiceExecute(object obj, Method method = Method.POST)
        {            
            var client = new RestClient(_baseUrl);
            var req = new RestSharp.RestRequest(_uri.AbsolutePath, method);

            req.JsonSerializer = new NewtonsoftJsonSerializer();
            req.AddJsonBody(obj);

            string json = req.JsonSerializer.Serialize(obj);
            Console.WriteLine(json);
            IRestResponse res = client.Execute(req);
            Console.WriteLine(res.Content);

            return res.Content;                                  
        }


    }
}
