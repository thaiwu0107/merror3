package com.merrors.merror.mvp.model;

import com.jess.arms.di.scope.ActivityScope;
import com.jess.arms.integration.IRepositoryManager;
import com.jess.arms.mvp.BaseModel;
import com.merrors.merror.mvp.contract.UserContract;
import com.merrors.merror.mvp.model.api.cache.CommonCache;
import com.merrors.merror.mvp.model.api.service.UserService;
import com.merrors.merror.mvp.model.entity.User;
import java.util.List;
import javax.inject.Inject;
import io.reactivex.Observable;
import io.rx_cache2.DynamicKey;
import io.rx_cache2.EvictDynamicKey;
import io.rx_cache2.Reply;

/**
 * Created by jess on 9/4/16 10:56
 * Contact with jess.yan.effort@gmail.com
 */
@ActivityScope
public class UserModel extends BaseModel implements UserContract.Model {
    public static final int USERS_PER_PAGE = 10;

    @Inject
    public UserModel(IRepositoryManager repositoryManager) {
        super(repositoryManager);
    }

    @Override
    public Observable<List<User>> getUsers(int lastIdQueried, boolean update) {
        //使用rxcache缓存,上拉刷新则不读取缓存,加载更多读取缓存
        return Observable.just(mRepositoryManager
                .obtainRetrofitService(UserService.class)
                .getUsers(lastIdQueried, USERS_PER_PAGE))
                .flatMap((Observable<List<User>> listObservable)->mRepositoryManager.obtainCacheService(CommonCache.class)
                        .getUsers(listObservable
                                , new DynamicKey(lastIdQueried)
                                , new EvictDynamicKey(update))
                        .map(Reply::getData));

    }

}
