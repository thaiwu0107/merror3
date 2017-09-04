package com.merrors.merror.mvp.model.oauth;

/**
 * Created by ggttoo44 on 2017/8/23.
 */

import com.github.scribejava.core.builder.api.DefaultApi20;
import com.github.scribejava.core.model.OAuth2AccessToken;
import io.reactivex.Observable;
import java.io.IOException;
import okhttp3.Interceptor;
import okhttp3.Request;
import okhttp3.Response;
import org.fuckboilerplate.rx_social_connect.NotActiveTokenFoundException;
import org.fuckboilerplate.rx_social_connect.RxSocialConnect;

public class OAuth2Interceptor implements Interceptor {
    private final Class<? extends DefaultApi20> apiClass;

    public OAuth2Interceptor(Class<? extends DefaultApi20> apiClass) {
        this.apiClass = apiClass;
    }

    @Override public Response intercept(Chain chain) throws IOException, NotActiveTokenFoundException {
        String token = getOToken().blockingFirst().getAccessToken();
        Request request = chain.request();

        Request.Builder builderRequest = request.newBuilder();
        builderRequest.addHeader("Authorization", "Bearer " + token);
        request = builderRequest.build();

        Response response = chain.proceed(request);

        return response;
    }

    //Exists for testing purposes
    protected Observable<OAuth2AccessToken> getOToken() {
        return RxSocialConnect.getTokenOAuth2(apiClass);
    }
}