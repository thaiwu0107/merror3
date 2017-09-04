package com.merrors.merror.di.module;

import com.jess.arms.di.scope.ActivityScope;

import dagger.Module;
import dagger.Provides;

import com.merrors.merror.mvp.contract.StartContract;
import com.merrors.merror.mvp.model.StartModel;


@Module
public class StartModule {
    private StartContract.View view;

    /**
     * 构建StartModule时,将View的实现类传进来,这样就可以提供View的实现类给presenter
     *
     * @param view
     */
    public StartModule(StartContract.View view) {
        this.view = view;
    }

    @ActivityScope
    @Provides
    StartContract.View provideStartView() {
        return this.view;
    }

    @ActivityScope
    @Provides
    StartContract.Model provideStartModel(StartModel model) {
        return model;
    }
}