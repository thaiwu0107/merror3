package com.merrors.merror.di.module;

import com.jess.arms.di.scope.ActivityScope;

import dagger.Module;
import dagger.Provides;

import com.merrors.merror.mvp.contract.ContainContract;
import com.merrors.merror.mvp.model.ContainModel;


@Module
public class ContainModule {
    private ContainContract.View view;

    /**
     * 构建ContainModule时,将View的实现类传进来,这样就可以提供View的实现类给presenter
     *
     * @param view
     */
    public ContainModule(ContainContract.View view) {
        this.view = view;
    }

    @ActivityScope
    @Provides
    ContainContract.View provideContainView() {
        return this.view;
    }

    @ActivityScope
    @Provides
    ContainContract.Model provideContainModel(ContainModel model) {
        return model;
    }
}